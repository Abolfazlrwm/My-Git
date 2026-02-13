#ifndef MYGIT_HPP
#define MYGIT_HPP

#include <string>
#include <vector>
#include <map>

//استراکچر برای سیو اطلاعات هر کامیت
struct Commit
{
    std::string id;
    std::string parent_id;
    std::string message;
    std::string timestamp;
    std::map<std::string, std::string> files;
};

class MyGit
{
public:
    MyGit();

    void init();
    void add(const std::string &filename);
    void commit(const std::string &message);
    void log();
    void checkout(const std::string &commit_hash);
    void status();

private:
    const std::string root_dir = ".mygit";
    const std::string objects_dir = ".mygit/objects";
    const std::string commits_dir = ".mygit/commits";
    const std::string head_file = ".mygit/HEAD";
    const std::string index_file = ".mygit/index";

    bool directory_exists(const std::string &path);
    void create_dir(const std::string &path);
    std::string hash_content(const std::string &content);
    std::string get_current_time();
    std::string read_file(const std::string &path);
    void write_file(const std::string &path, const std::string &content);

    std::map<std::string, std::string> load_index();
    void save_index(const std::map<std::string, std::string> &index);
    void save_commit_object(const Commit &commit);
    Commit load_commit_object(const std::string &hash);
    std::string get_head_commit_hash();
    void update_head(const std::string &commit_hash);
};

#endif