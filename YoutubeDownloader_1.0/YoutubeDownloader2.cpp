#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

class YoutubeDownloader {

private:
    int numberVideo;
    string link;
    string format;

public: 

    int GetNumberVideo(){
        return numberVideo;
    }

    string GetLink(){
        return link;
    }

    string GetFormat(){
        return format;
    }

    void SetNumberVideo(int valueNumberVideo){
        if (valueNumberVideo == valueNumberVideo*(-1) && valueNumberVideo > 10000){
            // return 1;
            cout << "Exeption! Number Video < 10 000";
        } else {
            numberVideo = valueNumberVideo;
        }
    }

    void SetLink(string valueLink){
        if (valueLink == " " && valueLink == ""){
            // return 1;
            cout << "Exeption! Link != " " ";
        } else {
            link = valueLink;
        }
    }

    void SetFormat(string valueFormat){
        if (valueFormat == " " && valueFormat == ""){
            // return 1;
             cout << "Exeption! Format != " " ";
        } else {
            format = valueFormat;  
        }
    }

    void Print() {
        cout << "Video: " + numberVideo << ": " + format + " " + link << endl; 
    }

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
        Video.SetNumberVideo(numberOfObjectsVideo + 1);

        cout << "Enter video link:" << endl;
        string EnteredLink;
        getline(cin, EnteredLink);

        Video.SetLink(EnteredLink);

        // Перевірка на погодження або вихід
        if (Video.GetLink() == "Yes" || Video.GetLink() == "yes" || Video.GetLink() == "Y" || Video.GetLink() == "y") {
            for (const auto& video : videos) {
                video.downloadVideo();
            }
            break;
        }
        if (Video.GetLink() == "No" || Video.GetLink() == "no" || Video.GetLink() == "N" || Video.GetLink() == "n") {
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
                Video.SetFormat(".mp4");
                break;
            case 2:
                Video.SetFormat(".webm");
                break;
            case 3:
                Video.SetFormat(".mkv");
                break;
            case 4:
                Video.SetFormat(".mp3");
                break;
            case 5:
                Video.SetFormat(".aac");
                break;
            case 6:
                Video.SetFormat(".m4a");
                break;
            default:
                cout << "Invalid choice. Defaulting to .mp4" << endl;
                Video.SetFormat(".mp4");
                break;
        }

        videos.push_back(Video);
        numberOfObjectsVideo++;

        cout << "You entered: " << Video.GetLink() + Video.GetFormat() << endl;
        cout << "Number of video: " << Video.GetNumberVideo() << endl;
    }


    // Виведення всіх введених відео
    // cout << "\nAll entered videos:" <<  << endl;
    // for (const auto& Video : videos) {
    //     cout << "Video number: " << Video.GetNumberVideo() << ", Link: " << Video.GetLink() << ", Format: " << Video.GetFormat() << endl;
    // }

    return 0;
}
