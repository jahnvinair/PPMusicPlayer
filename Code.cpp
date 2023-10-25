#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

using namespace std;

class Media {
protected:
    string title;

public:
    Media(const string& title) : title(title) {}

    virtual void play() {
        cout << "Playing: " << title << endl;
    }

    virtual void displayDetails() {
        cout << "Title: " << title << endl;
    }
};

class Song : public Media {
private:
    string artist;

public:
    Song(const string& title, const string& artist) : Media(title), artist(artist) {}

    void play() override {
        cout << "Playing Song: " << title << " by " << artist << endl;
    }

    void displayDetails() override {
        cout << "Title: " << title << ", Artist: " << artist << endl;
    }
};

class Playlist {
private:
    vector<Media*> tracks;
    vector<int> shuffledIndices;
    int currentTrackIndex = 0;

public:
    bool isShuffled = false;
    bool isRepeatOn = false;

    void addMedia(Media* media) {
        tracks.push_back(media);
        if (isShuffled) {
            shuffleIndices();
        }
    }

    void displayPlaylist() {
        cout << "Playlist:" << endl;
        for (int i = 0; i < tracks.size(); i++) {
            cout << i + 1 << ". ";
            if (i == currentTrackIndex) {
                cout << ">> ";
            }
            tracks[i]->displayDetails();
        }
    }

    void playCurrentTrack() {
        if (currentTrackIndex >= 0 && currentTrackIndex < tracks.size()) {
            tracks[currentTrackIndex]->play();
        } else {
            cout << "Invalid track selection." << endl;
        }
    }

    void moveNext() {
        if (isShuffled) {
            currentTrackIndex = getNextShuffledIndex();
        } else {
            if (currentTrackIndex < tracks.size() - 1) {
                currentTrackIndex++;
            } else if (isRepeatOn) {
                currentTrackIndex = 0;
            } else {
                cout << "End of playlist reached." << endl;
            }
        }
    }

    void movePrevious() {
        if (isShuffled) {
            currentTrackIndex = getPreviousShuffledIndex();
        } else {
            if (currentTrackIndex > 0) {
                currentTrackIndex--;
            } else if (isRepeatOn) {
                currentTrackIndex = tracks.size() - 1;
            } else {
                cout << "Start of playlist reached." << endl;
            }
        }
    }

    void toggleShuffle() {
        isShuffled = !isShuffled;
        if (isShuffled) {
            shuffleIndices();
        }
    }

    void toggleRepeat() {
        isRepeatOn = !isRepeatOn;
    }

    void shuffleIndices() {
        int numTracks = tracks.size();
        shuffledIndices.resize(numTracks);
        for (int i = 0; i < numTracks; i++) {
            shuffledIndices[i] = i;
        }
        random_shuffle(shuffledIndices.begin(), shuffledIndices.end());
    }

    int getNextShuffledIndex() {
        if (shuffledIndices.empty()) {
            return 0;
        }
        return shuffledIndices[currentTrackIndex];
    }

    int getPreviousShuffledIndex() {
        if (shuffledIndices.empty()) {
            return 0;
        }
        int currentIndex = currentTrackIndex;
        for (int i = 0; i < shuffledIndices.size(); i++) {
            if (shuffledIndices[i] == currentIndex) {
                return (i == 0) ? shuffledIndices[shuffledIndices.size() - 1] : shuffledIndices[i - 1];
            }
        }
        return shuffledIndices[0];
    }
};

int main() {
    vector<Playlist> playlists;
    int currentPlaylist = -1;
    int choice;

    cout << "Welcome to the Music Player!" << endl;

    while (true) {
        if (currentPlaylist != -1) {
            cout << "\nCurrent Playlist: " << currentPlaylist  << endl;
        }

        cout << "Main Menu:" << endl;
        cout << "1. Play" << endl;
        cout << "2. Next" << endl;
        cout << "3. Previous" << endl;
        cout << "4. Toggle Shuffle" << endl;
        cout << "5. Toggle Repeat" << endl;
        cout << "6. Create Playlist" << endl;
        cout << "7. Manage Playlists" << endl;

        if (currentPlaylist != -1) {
                cout << "8. Delete Playlist" << endl;
        }

        cout << "9. Exit" << endl;

        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        if (choice == 1) {
            if (currentPlaylist != -1) {
                playlists[currentPlaylist - 1].playCurrentTrack();
            } else {
                cout << "Please select a playlist first." << endl;
            }
        } else if (choice == 2) {
            if (currentPlaylist != -1) {
                playlists[currentPlaylist - 1].moveNext();
            } else {
                cout << "Please select a playlist first." << endl;
            }
        } else if (choice == 3) {
            if (currentPlaylist != -1) {
                playlists[currentPlaylist - 1].movePrevious();
            } else {
                cout << "Please select a playlist first." << endl;
            }
        } else if (choice == 4) {
            if (currentPlaylist != -1) {
                playlists[currentPlaylist - 1].toggleShuffle();
                cout << (playlists[currentPlaylist - 1].isShuffled ? "Shuffle is on." : "Shuffle is off.") << endl;
            } else {
                cout << "Please select a playlist first." << endl;
            }
        } else if (choice == 5) {
            if (currentPlaylist != -1) {
                playlists[currentPlaylist - 1].toggleRepeat();
                cout << (playlists[currentPlaylist - 1].isRepeatOn ? "Repeat is on." : "Repeat is off.") << endl;
            } else {
                cout << "Please select a playlist first." << endl;
            }
        } else if (choice == 6) {
            playlists.push_back(Playlist());
            currentPlaylist = playlists.size();
            cout << "New playlist created." << endl;
        } else if (choice == 7) {
            if (!playlists.empty()) {
                cout << "Select a playlist (1-" << playlists.size() << "): ";
                cin >> currentPlaylist;
                if (currentPlaylist < 1 || currentPlaylist > playlists.size()) {
                    cout << "Invalid playlist selection." << endl;
                    currentPlaylist = -1;
                }
            } else {
                cout << "No playlists available. Please create a playlist first." << endl;
            }
        } else if (choice == 8) {
            if (currentPlaylist != -1) {
                playlists.erase(playlists.begin() + currentPlaylist - 1);
                currentPlaylist = -1;
                cout << "Playlist deleted." << endl;
            } else {
                cout << "No playlist is currently selected." << endl;
            }
        } else if (choice == 9) {
            cout << "Thank you for using the Music Player. Goodbye!" << endl;
            return 0;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
