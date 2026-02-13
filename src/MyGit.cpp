#include "../include/MyGit.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

MyGit::MyGit() {}

bool MyGit::directory_exists(const string &path)
{
    struct stat info;
    return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

void MyGit::create_dir(const string &path)
{
    string command = "mkdir " + path + " > nul 2>&1";
    system(command.c_str());
}

void MyGit::init()
{
    if (directory_exists(root_dir))
    {
        cout << "Error: Repository already exists." << endl;
        return;
    }
    create_dir(objects_dir);
    create_dir(commits_dir);
    write_file(head_file, "");
    cout << "Initialized empty MyGit repository in .mygit/" << endl;
}

void MyGit::add(const string &filename)
{
    string content = read_file(filename);
    if (content.empty() && fstream(filename).fail())
    {
        cerr << "Error: File '" << filename << "' not found." << endl;
        return;
    }

    string file_hash = hash_content(content);
    write_file(objects_dir + "/" + file_hash, content);

    auto index = load_index();
    index[filename] = file_hash;
    save_index(index);
    cout << "Staged file: " << filename << endl;
}

void MyGit::commit(const string &message)
{
    auto index = load_index();
    if (index.empty())
    {
        cout << "Nothing to commit (staging area empty)." << endl;
        return;
    }

    Commit c;
    c.parent_id = get_head_commit_hash();
    c.message = message;
    c.timestamp = get_current_time();
    c.files = index;

    stringstream ss;
    ss << c.parent_id << c.message << c.timestamp;
    c.id = hash_content(ss.str());

    save_commit_object(c);
    update_head(c.id);
    cout << "[" << c.id.substr(0, 7) << "] " << message << endl;
}

void MyGit::log()
{
    string current = get_head_commit_hash();
    if (current.empty())
    {
        cout << "No commits found." << endl;
        return;
    }

    while (!current.empty() && current != "\n")
    {
        Commit c = load_commit_object(current);
        if (c.id.empty())
            break;
        cout << "Commit:  " << c.id << endl;
        cout << "Date:    " << c.timestamp << endl;
        cout << "Message: " << c.message << endl;
        cout << "-----------------------------------" << endl;
        current = c.parent_id;
    }
}

void MyGit::checkout(const string &hash)
{
    Commit c = load_commit_object(hash);
    if (c.id.empty())
    {
        cerr << "Error: Commit " << hash << " not found." << endl;
        return;
    }

    for (auto const &item : c.files)
    {
        string content = read_file(objects_dir + "/" + item.second);
        write_file(item.first, content);
    }
    update_head(hash);
    save_index(c.files);
    cout << "Checked out to: " << hash.substr(0, 7) << endl;
}


string MyGit::hash_content(const string &content)
{
    size_t h = hash<string>{}(content);
    stringstream ss;
    ss << hex << h;
    return ss.str();
}

string MyGit::get_current_time()
{
    time_t now = time(0);
    string dt = ctime(&now);
    if (!dt.empty())
        dt.pop_back();
    return dt;
}

string MyGit::read_file(const string &path)
{
    ifstream f(path.c_str(), ios::binary);
    stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void MyGit::write_file(const string &path, const string &content)
{
    ofstream f(path.c_str(), ios::binary);
    f << content;
}

map<string, string> MyGit::load_index()
{
    map<string, string> idx;
    ifstream f(index_file.c_str());
    string name, hash_val;
    while (f >> name >> hash_val)
        idx[name] = hash_val;
    return idx;
}

void MyGit::save_index(const map<string, string> &idx)
{
    ofstream f(index_file.c_str());
    for (auto const &item : idx)
        f << item.first << " " << item.second << endl;
}

void MyGit::save_commit_object(const Commit &c)
{
    ofstream f((commits_dir + "/" + c.id).c_str());
    f << c.parent_id << "\n"
      << c.timestamp << "\n"
      << c.message << "\n--FILES--\n";
    for (auto const &item : c.files)
        f << item.first << " " << item.second << endl;
}

Commit MyGit::load_commit_object(const string &hash)
{
    Commit c;
    ifstream f((commits_dir + "/" + hash).c_str());
    if (!f)
        return c;
    c.id = hash;
    getline(f, c.parent_id);
    getline(f, c.timestamp);
    getline(f, c.message);
    string line;
    while (getline(f, line) && line != "--FILES--")
        ;
    string n, h_val;
    while (f >> n >> h_val)
        c.files[n] = h_val;
    return f.close(), c;
}

string MyGit::get_head_commit_hash()
{
    string h = read_file(head_file);
    h.erase(remove(h.begin(), h.end(), '\n'), h.end());
    h.erase(remove(h.begin(), h.end(), '\r'), h.end());
    return h;
}

void MyGit::update_head(const string &hash)
{
    write_file(head_file, hash);
}

void MyGit::status()
{
    auto idx = load_index();
    cout << "On branch master\nStaged files:\n";
    if (idx.empty())
        cout << "  (nothing to show)" << endl;
    for (auto const &item : idx)
        cout << "  modified: " << item.first << endl;
}