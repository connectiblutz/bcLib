#include "windowsservice.h"

#include <windows.h>

#define UNUSED(x) (void)(x)

namespace apfd::common {

namespace singleton {
  std::weak_ptr<WindowsService> _;
}


SERVICE_STATUS          ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;

BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	BOOL fResult = TRUE;

	if (dwCurrentState == SERVICE_START_PENDING)
		ssStatus.dwControlsAccepted = 0;
	else
		ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_POWEREVENT;

	ssStatus.dwCurrentState = dwCurrentState;
	ssStatus.dwWin32ExitCode = dwWin32ExitCode;
	ssStatus.dwWaitHint = dwWaitHint;

	if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
		ssStatus.dwCheckPoint = 0;
	else
		ssStatus.dwCheckPoint = dwCheckPoint++;

	// Report the status of the service to the service control manager.
	fResult = SetServiceStatus(sshStatusHandle, &ssStatus);

	return fResult;
}

VOID ServiceStop()
{
	ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 3000);
  auto s = singleton::_.lock();
	if (s) {
    s->thread()->stop();
  }
}


VOID ServiceStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
  UNUSED(dwArgc);
  UNUSED(lpszArgv);

	// Service initialization
	if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 3000))
	{
		ServiceStop();
		return;
	}

	std::thread mainThread([] {
    auto s = singleton::_.lock();
	   if (s) {
       s->thread()->join();
     }
  });

	if (!ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 0))
	{
		ServiceStop();
		return;
	}
	// End of initialization


	// need to idle in ServiceStart so Windows knows the service is running
	mainThread.join();
}


DWORD WINAPI service_ctrl( __in  DWORD dwCtrlCode, __in  DWORD dwEventType, __in  LPVOID lpEventData, __in  LPVOID lpContext)
{
  UNUSED(dwEventType);
  UNUSED(lpEventData);
  UNUSED(lpContext);

	DWORD retCode = NO_ERROR;
	// Handle the requested control code.
	switch (dwCtrlCode)
	{
		// Stop the service.
		// SERVICE_STOP_PENDING should be reported before setting the Stop Event - hServerStopEvent - in
		// ServiceStop().  This avoids a race condition which may result in a 1053 - The Service did not respond... error.
	case SERVICE_CONTROL_STOP:
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 0);
		ServiceStop();
		retCode = NO_ERROR;
		return retCode;

		// Update the service status.
	case SERVICE_CONTROL_INTERROGATE:
		ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
		retCode = NO_ERROR;
		break;
	default:
    {
    }
		break;

	}

	return retCode;
}


void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv)
{
	// register our service control handler:
  std::string name;
  auto s = singleton::_.lock();
	if (s) {
    name = s->name();
  }
	sshStatusHandle = RegisterServiceCtrlHandlerExA(name.c_str(), service_ctrl, NULL);

	if (!sshStatusHandle)
	{
		goto cleanup;
	}

	// SERVICE_STATUS members that don't change in example
	ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ssStatus.dwServiceSpecificExitCode = 0;

	// report the status to the service control manager.
	if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 3000))
	{
		goto cleanup;
	}

	ServiceStart(dwArgc, lpszArgv);

	ReportStatusToSCMgr(SERVICE_STOPPED, NO_ERROR, 0);


cleanup:

	return;
}

WindowsService::WindowsService(std::string name,std::shared_ptr<MessageThread> thread) {
  singleton::_=std::shared_ptr<WindowsService>(this);
  _name=name;
  _thread=thread;

	SERVICE_TABLE_ENTRY dispatchTable[] =
	{
		{ (const LPSTR) _name.c_str(), (LPSERVICE_MAIN_FUNCTION)service_main },
		{ NULL, NULL }
	};

	StartServiceCtrlDispatcher(dispatchTable);
}

template<class... Args>
std::shared_ptr<WindowsService> WindowsService::make_shared(Args&&... args)
{
  auto obj = new WindowsService(std::forward<Args>(args)...);
  auto made = std::shared_ptr<WindowsService>(obj,D());
  singleton::_=made;
  return made;
}
template std::shared_ptr<WindowsService> WindowsService::make_shared<std::string,std::shared_ptr<MessageThread>>(std::string&&,std::shared_ptr<MessageThread>&&);

}