#include <iostream>
#include <boost/process.hpp>

using namespace boost::process;

using WordData = std::vector<std::pair<std::string, uint64_t>>;

std::string GetReduceFileName(const std::string& type, uint64_t file_num) {
    return "reduce_job_" + type + "_" + std::to_string(file_num) + ".txt";
}

void RunReduceJob(const std::string& script_path, const WordData& data, uint64_t file_num) {
    const auto file_name = GetReduceFileName("input", file_num);

    std::freopen(file_name.data(), "w", stdout);
    for (const auto& it : data) {
        std::cout << it.first << " " << it.second << std::endl;
    }

    const auto out_file_name = GetReduceFileName("output", file_num);
    system(script_path, std_out > out_file_name, std_in < file_name);
}

struct TMapReduceOptions {
    using Str = std::string;

    Str type;
    Str script_path;
    Str src_file;
    Str dst_file;
};

void RunReduce(const TMapReduceOptions& options) {
    WordData words;
    {
        std::string word;
        uint64_t cnt;
        std::ifstream stream(options.src_file);
        while (stream >> word >> cnt) {
            words.emplace_back(word, cnt);
        }
    }

    std::sort(words.begin(), words.end());

    uint64_t file_num = 0;
    WordData data;
    for (const auto& item : words) {
        const auto& word = item.first;
        const auto cnt = item.second;
        if (data.empty() || data.back().first == word) {
            data.emplace_back(word, cnt);
            continue;
        }

        if (data.back().first != word) {
            RunReduceJob(options.script_path, data, ++file_num);
            data.clear();
            data.emplace_back(word, cnt);
        }
    }
    RunReduceJob(options.script_path, data, ++file_num);

    // cat file_1 file_2 ...
    std::string command = "cat";
    for (int i = 1; i <= file_num; ++i) {
        const auto file_name = GetReduceFileName("output", i);
        command += " " + file_name;
    }

    system(command, std_out > options.dst_file);

    for (int i = 1; i <= file_num; ++i) {
        const auto inp_file_name = GetReduceFileName("input", i);
        const auto out_file_name = GetReduceFileName("output", i);
        std::remove(inp_file_name.data());
        std::remove(out_file_name.data());
    }
}

void RunMap(const TMapReduceOptions& options) {
    system(options.script_path, std_out > options.dst_file, std_in < options.src_file);
}

void RunMapReduce(const TMapReduceOptions& options) {
    if (options.type == "map") {
        RunMap(options);
    } else if (options.type == "reduce") {
        RunReduce(options);
    } else {
        throw std::runtime_error("Unknown type: " + options.type);
    }
}

int main(int num_args, char* arg_values[]) {
    if (num_args != 5) {
        throw std::runtime_error("MapReduce must take 5 arguments");
    }

    TMapReduceOptions options;
    options.type = arg_values[1];
    options.script_path = arg_values[2];
    options.src_file = arg_values[3];
    options.dst_file = arg_values[4];

    RunMapReduce(options);

    return 0;
}
