#pragma once
#include <string>

const int MAX_LEADERBOARD_ENTRIES = 10;

struct LeaderboardEntry
{
    char username[64];
    int score;
    int levelReached;
    char date[32];
};

struct UserRecord
{
    char username[64];
    char passwordHash[128];
    int currentLevel;
    int lives;
    int gems;
    int highScore;
};

class FileManager
{
public:
    static FileManager& getinstance();

    bool registerUser(const std::string& username, const std::string& password);
    bool loginUser(const std::string& username, const std::string& password);
    bool saveProgress(const std::string& username, int level, int lives, int gems, int score);
    bool loadProgress(const std::string& username, UserRecord& out);
    bool userExists(const std::string& username);

    bool saveLeaderboardEntry(const std::string& username, int score, int levelReached);
    int  loadleaderboard(LeaderboardEntry* entries, int maxCount);

    std::string getLoggedInUser() const;
    void logout();
    bool isLoggedIn() const;

private:
    FileManager();
    FileManager(const FileManager&);
    FileManager& operator=(const FileManager&);

    std::string loggedInUser;
    bool loggedIn;

    std::string hashPassword(const std::string& password);
    std::string getUserFilePath(const std::string& username);
    bool writeUserRecord(const UserRecord& record);
    bool readUserRecord(const std::string& username, UserRecord& out);

    std::string getLeaderboardFilePath();
    void sortleaderboard(LeaderboardEntry* entries, int count);
};