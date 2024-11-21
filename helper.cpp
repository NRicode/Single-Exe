#include <string>
#include <random>

//can move some variables out but most likely won't matter anyway
std::string generateRandomString(size_t length) {

    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;  
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

    std::string randomString;
    for (size_t i = 0; i < length; ++i) {
        randomString += characters[distribution(generator)];
    }
    return randomString;
}


void replaceSubstring(std::string &str, const std::string &old_sub, const std::string &new_sub) {

    size_t pos = 0;
    while ((pos = str.find(old_sub, pos)) != std::string::npos) {
        str.replace(pos, old_sub.length(), new_sub);
        pos += new_sub.length();
    }

}

std::string removeFirstWord(const std::string str) {
    size_t pos = str.find(' ');
    if (pos == std::string::npos) {
        return "";
    }
    return str.substr(pos + 1);
}

std::string getFirstWord(const std::string& str) {
    size_t pos = str.find(' ');
    if (pos == std::string::npos) {
        return str; 
    }
    return str.substr(0, pos);
}

