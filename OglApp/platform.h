#include <chrono>

unsigned int get_timestamp(){
    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );
    return ms.count();
}

using namespace std;

string get_folder_from_filename(string filename){
    // Too lazy to use stl
    int size = filename.size();
    int end = 0;
    for(int i = size - 1; i > 1 && end == 0; i--){
        if(filename[i + 1] == '/'){
            end = i + 1;
        }
    }
    return filename.substr(0,end);
}
