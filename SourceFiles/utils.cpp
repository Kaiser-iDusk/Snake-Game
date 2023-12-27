#include<fstream>
#include<string>

typedef unsigned int u32;
typedef char s8;
typedef unsigned char u8;

#define internal static;
#define global_variable static;

inline int clamp(int min, int x, int max) {
    if (x > max)
        return max;
    if (x < min)
        return min;
    return x;
}

static void start_game() {
    using namespace std;

    ofstream games_write("games.txt", ofstream::out | ofstream::app);
    if (games_write.is_open())
        games_write.close();
}

static int score_extract(std::string &line) {
    using namespace std;

    string sbstr = line.substr(0, 3);
    //cout << sbstr << endl;
    int ans = 0;
    for (int i = 0; i< 3; i++) {
        ans += (sbstr[i] - '0');
        ans *= 10;
    }
    return ans / 10;
}

static std::string char_extract(std::string& line) {
    using namespace std;
    return line.substr(4);
}

static void read_scores(std::priority_queue<std::pair<int, std::string>>& pq) {
    using namespace std;

    ifstream games_read("games.txt");
    string line;
    if (games_read.is_open()) {
        while (getline(games_read, line)) {
            //cout << line << endl;
            int int_part = score_extract(line);
            string char_part = char_extract(line);
            //cout << char_part << endl;
            pq.push(make_pair(int_part, char_part));
        }
        games_read.close();
    }
    else {
        cout << "Error: Unable to write to file games.txt" << endl;
    }
}

static std::string make_string_to_int(int val) {
    using namespace std;

    string ans = "000";
    int i = 2;
    while (val) {
        ans[i--] += (val % 10);
        val /= 10;
    }
    return ans;
}
static void make_highscores(std::priority_queue<std::pair<int, std::string>>& pq, int count) {
    using namespace std;

    ofstream writer("highscores.txt", ofstream::out);
    if (writer.is_open()) {
        while (!pq.empty() && (count--)) {
            pair<int, string> p = pq.top();
            pq.pop();
            string str_score = make_string_to_int(p.first);
            writer << (str_score + "|" + p.second) << endl;
        }
        writer.close();
    }
    else {
        cout << "Error: Unable to lock/open highscores.txt" << endl;
    }
}
static void deleteDB() {
    using namespace std;

    std::ofstream ofs;
    ofs.open("games.txt", ofstream::out | ofstream::trunc);
    ofs.close();
}
static void update_game(std::pair<int, std::string>& p) {
    using namespace std;

    ofstream games_write("games.txt", ofstream::out | ofstream::app);
    if (games_write.is_open()) {
        string str_score = make_string_to_int(p.first);
        games_write << (str_score + "|" + p.second) << endl;
        games_write.close();
    }
    else {
        cout << "Error: Cannot hold a lock on games.txt" << endl;
    }
}
static std::string char_to_string(char* inputBox) {
    using namespace std;

    string ans = "";
    int i = 0;
    while (inputBox[i] != '\0') {
        ans += inputBox[i];
        i++;
    }
    return ans;
}

static void give_leaderboard(std::string& str) {
    using namespace std;

    ifstream reader("highscores.txt");
    if (reader.is_open()) {
        string line;
        while (getline(reader, line)) {
            int scr = score_extract(line);
            string chr = char_extract(line);

            str += (make_string_to_int(scr) + "   " + chr + "\n");
        }
        reader.close();
    }
    else {
        cout << "Error: Unable to lock/open highscores.txt" << endl;
    }
}