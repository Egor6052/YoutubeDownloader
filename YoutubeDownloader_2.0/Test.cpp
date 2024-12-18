#include "imgui/imgui.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <future>
#include <cstdio>

using namespace std;

class YoutubeDownloader {
public:
    int numberVideo;
    string link;
    string format;
    bool isDownloading = false;
    bool isCompleted = false;
    string downloadProgress;

    void downloadVideo() {
        isDownloading = true;
        isCompleted = false;

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

        // Виконання команди через пайп
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            cerr << "Failed to run command." << endl;
            return;
        }

        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            downloadProgress = buffer;
        }

        pclose(pipe);

        isDownloading = false;
        isCompleted = true;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 400), "YoutubeDownloader", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    char url[256] = "";  // Буфер для введення URL
    vector<YoutubeDownloader> videos;
    int numberOfObjectsVideo = 1;
    int formatChoice = 0;
    const char* formats[] = { ".mp4", ".webm", ".mkv", ".mp3", ".aac", ".m4a" };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        window.clear(sf::Color::Black);

        // Отримання розмірів вікна
        ImVec2 windowSize = ImGui::GetIO().DisplaySize;

        // Центрування головного вікна
        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 380, windowSize.y / 2 - 150), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(760, 140), ImGuiCond_Always);

        ImGui::Begin("##Main", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); // Встановлення червоного кольору тексту
        ImGui::Text("Enter video link:");
        ImGui::PopStyleColor();

        // Встановлення ширини поля введення
        ImGui::SetNextItemWidth(500);
        ImGui::InputText("##URL", url, sizeof(url));
        
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        ImGui::Text("Select format:");
        ImGui::PopStyleColor();
        
        ImGui::SetNextItemWidth(200);
        ImGui::Combo("##Format", &formatChoice, formats, IM_ARRAYSIZE(formats));

        YoutubeDownloader video;

        // if (ImGui::Button("Add Video")) {
        //     video.numberVideo = numberOfObjectsVideo + 1;
        //     // video.link = string(url);
        //     // video.format = formats[formatChoice];
        //     // videos.push_back(video);
        //     numberOfObjectsVideo++;
        //     url[0] = '\0';  // Очищення поля введення
        // }
        // ImGui::SameLine();

        if (ImGui::Button("Download")) {
            video.numberVideo = numberOfObjectsVideo;
            video.link = string(url);
            video.format = formats[formatChoice];
            videos.push_back(video);
            numberOfObjectsVideo++;
            for (auto& video : videos) {
                std::thread(&YoutubeDownloader::downloadVideo, &video).detach();   
            }
        }

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        if (ImGui::Button("Clean Videos..")) {
            videos.clear();
            numberOfObjectsVideo = 0;
            url[0] = '\0';
        }
        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        if (ImGui::Button("Stop")) {
            return 0;
        }
        ImGui::PopStyleColor();


        ImGui::End();

        // Центрування вікна з інформацією
        ImGui::SetNextWindowPos(ImVec2(windowSize.x / 2 - 380, windowSize.y / 2), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(760, 200), ImGuiCond_Always);
        ImGui::Begin("Video Info", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        // Відображення інформації про відео
        ImGui::Text("Videos to download:");
        for (auto& video : videos) {
            ImGui::BulletText("Video %d: %s [%s]", video.numberVideo, video.link.c_str(), video.format.c_str());
            if (video.isDownloading) {
                ImGui::Text("Status: Downloading...");
                ImGui::Text("Progress: %s", video.downloadProgress.c_str());
            } else if (video.isCompleted) {
                ImGui::Text("Status: Download Completed");
            }
        }

        ImGui::End();

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
