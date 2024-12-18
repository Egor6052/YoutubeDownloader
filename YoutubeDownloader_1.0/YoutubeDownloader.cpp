#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

class YoutubeDownloader {
public:
    int numberVideo;
    string link;
    string format;

    void downloadVideo() const {
        // Очищення URL від параметрів
        string cleanedLink = link.substr(0, link.find('?'));

        // Команда для завантаження відео або аудіо
        string command;
        if (format == ".mp3" || format == ".aac" || format == ".m4a") {
            command = "yt-dlp -x --audio-format " + format.substr(1) + " " + cleanedLink + " -o '%(title)s_" + to_string(numberVideo) + format + "'";
        } else {
            command = "yt-dlp " + cleanedLink + " -o '%(title)s_" + to_string(numberVideo) + format + "'";
        }

        cout << "Executing command: " << command << endl;
        system(command.c_str());
    }
};

int main() {
    vector<YoutubeDownloader> videos;
    int numberOfObjectsVideo = 0;

    cout << "YoutubeDownloader" << endl << "---for download video, enter the link and enter Yes or No---" << endl;

    while (true) {
        YoutubeDownloader Video;
        Video.numberVideo = numberOfObjectsVideo + 1;

        cout << "Enter video link:" << endl;
        getline(cin, Video.link);

        // Перевірка на погодження або вихід
        if (Video.link == "Yes" || Video.link == "yes" || Video.link == "Y" || Video.link == "y") {
            for (const auto& video : videos) {
                video.downloadVideo();
            }
            break;
        }
        if (Video.link == "No" || Video.link == "no" || Video.link == "N" || Video.link == "n") {
            break;
        }

        // Обрання формату
        int choice;
        cout << "Enter the format:" << endl 
             << "1 - .mp4" << endl 
             << "2 - .webm" << endl 
             << "3 - .mkv" << endl 
             << "4 - .mp3" << endl 
             << "5 - .aac" << endl 
             << "6 - .m4a" << endl;

        while (true) {
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number from 1 to 6." << endl;
            } else if (choice < 1 || choice > 6) {
                cout << "Invalid choice. Please enter a number from 1 to 6." << endl;
            } else {
                break;
            }
        }
        cin.ignore();

        switch (choice) {
            case 1:
                Video.format = ".mp4";
                break;
            case 2:
                Video.format = ".webm";
                break;
            case 3:
                Video.format = ".mkv";
                break;
            case 4:
                Video.format = ".mp3";
                break;
            case 5:
                Video.format = ".aac";
                break;
            case 6:
                Video.format = ".m4a";
                break;
            default:
                cout << "Invalid choice. Defaulting to .mp4" << endl;
                Video.format = ".mp4";
                break;
        }

        videos.push_back(Video);
        numberOfObjectsVideo++;

        cout << "You entered: " << Video.link << endl;
        cout << "Number of video: " << Video.numberVideo << endl;
    }


    // Виведення всіх введених відео
    cout << "\nAll entered videos:" << endl;
    for (const auto& Video : videos) {
        cout << "Video number: " << Video.numberVideo << ", Link: " << Video.link << ", Format: " << Video.format << endl;
    }

    return 0;
}
