#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

// Function to scramble a word
string scrambleWord(string word) {
    static random_device rd;
    static mt19937 g(rd());
    shuffle(word.begin(), word.end(), g);
    return word;
}

// Function to show leaderboard
void showLeaderboard() {
    ifstream inFile("leaderboard.txt");
    if (!inFile.is_open()) {
        cout << "\nNo leaderboard data found.\n";
        return;
    }

    vector<pair<string, int>> entries;
    string name;
    int score;

    while (inFile >> name >> score) {
        entries.push_back({name, score});
    }
    inFile.close();

    // Sort by score descending
    sort(entries.begin(), entries.end(), [](auto &a, auto &b) {
        return a.second > b.second;
    });

    cout << "\n🏆 Leaderboard (Top 3):\n";
    for (int i = 0; i < min(3, (int)entries.size()); ++i) {
        cout << i + 1 << ". " << entries[i].first << " - " << entries[i].second << " pts\n";
    }
    cout << "--------------------------\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed for fallback rand()

    vector<string> wordList = {
        "code", "puzzle", "table", "orange", "paper"
    };

    int score = 0;
    string playerName;

    cout << "🧩 Welcome to the Word Puzzle Game! 🧩\n\n";
    cout << "Enter your name: ";
    getline(cin >> ws, playerName);  // Supports full names with spaces

    cout << "\nHello " << playerName << "! Let's begin.\n\n";

    while (!wordList.empty()) {
        int index = rand() % wordList.size();
        string original = wordList[index];
        wordList.erase(wordList.begin() + index);

        string scrambled = scrambleWord(original);

        // Ensure it's not the same as original (with limited retries)
        int attempts = 0;
        while (scrambled == original && attempts < 5) {
            scrambled = scrambleWord(original);
            attempts++;
        }

        cout << "Guess the word: " << scrambled << endl;

        string guess;
        cout << "Your guess: ";
        cin >> guess;

        // Convert guess to lowercase
        transform(guess.begin(), guess.end(), guess.begin(),
                  [](unsigned char c) { return tolower(c); });

        if (guess == original) {
            cout << "✅ Correct!\n";
            score += 10;
        } else {
            cout << "❌ Wrong! The correct word was: " << original << endl;
        }
    }

    cout << "\n🎉 Final Score: " << score << "\n";

    // Save to leaderboard
    ofstream outFile("leaderboard.txt", ios::app);
    if (outFile.is_open()) {
        outFile << playerName << " " << score << "\n";
        outFile.close();
    } else {
        cout << "⚠️ Unable to save leaderboard.\n";
    }

    // Show leaderboard
    showLeaderboard();

    cout << "Thanks for playing!\n";
    return 0;
}
