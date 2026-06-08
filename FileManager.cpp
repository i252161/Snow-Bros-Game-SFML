#include "Constants.h"
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

FileManager::FileManager()
{
    loggedIn = false;
    loggedInUser = "";
}

FileManager& FileManager::getinstance()
{
    static FileManager instance;
    return instance;
}
string FileManager::hashPassword(const string& password)
{
    unsigned long hash = 5381;
    for (int i = 0; i < (int)password.size(); i++)
    {
        hash = ((hash << 5) + hash) + (unsigned char)password[i];
    }

    ostringstream oss;
    oss << hash;

    unsigned long hash2 = 0;
    for (int i = 0; i < (int)password.size(); i++)
    {
        hash2 += (unsigned char)password[i] * (i + 7);
        hash2 ^= (hash2 >> 3);
    }
    oss << "_" << hash2;

    return oss.str();
}

std::string FileManager::getUserFilePath(const std::string& username)
{
    return "Data/" + username + ".dat";
}
bool FileManager::writeUserRecord(const UserRecord& record)
{
    std::string path = getUserFilePath(record.username);
    std::ofstream file(path.c_str());
    if (!file.is_open())
        return false;

    file << record.username << "\n";
    file << record.passwordHash << "\n";
    file << record.currentLevel << "\n";
    file << record.lives << "\n";
    file << record.gems << "\n";
    file << record.highScore << "\n";
    file.close();
    return true;
}

bool FileManager::readUserRecord(const std::string& username, UserRecord& out)
{
    string path = getUserFilePath(username);
    ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    string line;

    getline(file, line);
    ::strncpy_s(out.username, sizeof(out.username), line.c_str(), _TRUNCATE);

    getline(file, line);
    ::strncpy_s(out.passwordHash, sizeof(out.passwordHash), line.c_str(), _TRUNCATE);
    getline(file, line);
    out.currentLevel = std::stoi(line);
    getline(file, line);
    out.lives = std::stoi(line);
    getline(file, line);
    out.gems = std::stoi(line);
    getline(file, line);
    out.highScore = std::stoi(line);
    file.close();
    return true;
}

bool FileManager::userExists(const std::string& username)
{
    std::ifstream file(getUserFilePath(username).c_str());
    return file.is_open();
}

bool FileManager::registerUser(const std::string& username, const std::string& password)
{
    if (userExists(username))
        return false;

    if (username.empty() || password.empty())
        return false;

    UserRecord record;
    ::strncpy_s(record.username, sizeof(record.username), username.c_str(), _TRUNCATE);
    std::string hashed = hashPassword(password);
    ::strncpy_s(record.passwordHash, sizeof(record.passwordHash), hashed.c_str(), _TRUNCATE);
    record.currentLevel = 1;
    record.lives = START_LIVES;
    record.gems = 0;
    record.highScore = 0;

    return writeUserRecord(record);
}

bool FileManager::loginUser(const std::string& username, const std::string& password)
{
    UserRecord record;
    if (!readUserRecord(username, record))
        return false;

    std::string hashed = hashPassword(password);
    if (hashed != std::string(record.passwordHash))
        return false;

    loggedInUser = username;
    loggedIn = true;
    return true;
}

bool FileManager::saveProgress(const std::string& username, int level, int lives, int gems, int score)
{
    UserRecord record;
    if (!readUserRecord(username, record))
        return false;

    record.currentLevel = level;
    record.lives = lives;
    record.gems = gems;
    if (score > record.highScore)
        record.highScore = score;

    return writeUserRecord(record);
}

bool FileManager::loadProgress(const std::string& username, UserRecord& out)
{
    return readUserRecord(username, out);
}
string FileManager::getLoggedInUser() const { return loggedInUser; }
void FileManager::logout() { loggedIn = false; loggedInUser = ""; }
bool FileManager::isLoggedIn() const { return loggedIn; }

string FileManager::getLeaderboardFilePath()
{
    return "Data/leaderboard.dat";
}
void FileManager::sortleaderboard(LeaderboardEntry* entries, int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (entries[j].score < entries[j + 1].score)
            {
                LeaderboardEntry tmp = entries[j];
                entries[j] = entries[j + 1];
                entries[j + 1] = tmp;
            }
        }
    }
}
bool FileManager::saveLeaderboardEntry(const std::string& username, int score, int levelReached)
{
    LeaderboardEntry entries[MAX_LEADERBOARD_ENTRIES];
    int count = loadleaderboard(entries, MAX_LEADERBOARD_ENTRIES);
    LeaderboardEntry newEntry;
    ::strncpy_s(newEntry.username, sizeof(newEntry.username), username.c_str(), _TRUNCATE);
    newEntry.score = score;
    newEntry.levelReached = levelReached;
    ::strncpy_s(newEntry.date, sizeof(newEntry.date), "2026", _TRUNCATE);

    bool replaced = false;
    for (int i = 0; i < count; i++)
    {
        if (std::string(entries[i].username) == username)
        {
            if (score > entries[i].score)
                entries[i] = newEntry;
            replaced = true;
            break;
        }
    }

    if (!replaced)
    {
        if (count < MAX_LEADERBOARD_ENTRIES)
        {
            entries[count] = newEntry;
            count++;
        }
        else
        {
            sortleaderboard(entries, count);
            if (score > entries[count - 1].score)
                entries[count - 1] = newEntry;
        }
    }

    sortleaderboard(entries, count);

    ofstream file(getLeaderboardFilePath().c_str());
    if (!file.is_open())
        return false;

    file << count << "\n";
    for (int i = 0; i < count; i++)
    {
        file << entries[i].username << "\n";
        file << entries[i].score << "\n";
        file << entries[i].levelReached << "\n";
        file << entries[i].date << "\n";
    }

    file.close();
    return true;
}

int FileManager::loadleaderboard(LeaderboardEntry* entries, int maxCount)
{
    ifstream file(getLeaderboardFilePath().c_str());
    if (!file.is_open())
        return 0;

    int count = 0;
    string line;
    getline(file, line);
    count = stoi(line);

    if (count > maxCount)
        count = maxCount;

    for (int i = 0; i < count; i++)
    {
        getline(file, line);
        ::strncpy_s(entries[i].username, sizeof(entries[i].username), line.c_str(), _TRUNCATE);

        getline(file, line);
        entries[i].score = stoi(line);

        getline(file, line);
        entries[i].levelReached = stoi(line);

        getline(file, line);
        ::strncpy_s(entries[i].date, sizeof(entries[i].date), line.c_str(), _TRUNCATE);
    }

    file.close();
    return count;
}