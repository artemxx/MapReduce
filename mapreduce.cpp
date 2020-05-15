#include <boost/process.hpp>
#include <iostream>

namespace bp = boost::process;

using WordData = std::vector<std::pair<std::string, uint64_t>>;

std::string GetReduceFileName(const std::string& type, uint64_t file_num) {
    return "reduce_job_" + type + "_" + std::to_string(file_num) + ".txt";
}

bp::child RunReduceJob(const std::string& script_path, const WordData& data, uint64_t file_num) {
    const auto file_name = GetReduceFileName("input", file_num);

    std::freopen(file_name.data(), "w", stdout);
    for (const auto& it : data) {
        std::cout << it.first << "\t" << it.second << std::endl;
    }

    const auto out_file_name = GetReduceFileName("output", file_num);
    return bp::child(script_path, bp::std_out > out_file_name, bp::std_in < file_name);
}

struct TMapReduceOptions {
    std::string type;
    std::string script_path;
    std::string src_file;
    std::string dst_file;
};

void AppendReduceProcess(const TMapReduceOptions& options, std::vector<bp::child>& processes,
                         const WordData& data, uint64_t& file_num) {
    auto child = RunReduceJob(options.script_path, data, ++file_num);
    processes.push_back(std::move(child));
}

uint64_t SplitIntoJobs(const TMapReduceOptions& options, const WordData& words) {
    uint64_t file_num = 0;
    WordData data;
    std::vector<bp::child> processes;
    for (const auto& item : words) {
        const auto& word = item.first;
        const auto cnt = item.second;
        if (data.empty() || data.back().first == word) {
            data.emplace_back(word, cnt);
            continue;
        }

        if (data.back().first != word) {
            AppendReduceProcess(options, processes, data, file_num);
            data.clear();
            data.emplace_back(word, cnt);
        }
    }
    AppendReduceProcess(options, processes, data, file_num);

    for (auto& process : processes) {
        process.wait();
    }

    return file_num;
}

void MergeOutputs(const TMapReduceOptions& options, uint64_t file_num) {
    // cat file_1 file_2 ...
    std::string command = "cat";
    for (int i = 1; i <= file_num; ++i) {
        const auto file_name = GetReduceFileName("output", i);
        command += " " + file_name;
    }

    bp::system(command, bp::std_out > options.dst_file);
}

void RemoveTempFiles(uint64_t file_num) {
    for (int i = 1; i <= file_num; ++i) {
        const auto inp_file_name = GetReduceFileName("input", i);
        const auto out_file_name = GetReduceFileName("output", i);
        std::remove(inp_file_name.data());
        std::remove(out_file_name.data());
    }
}

WordData ScanWords(const TMapReduceOptions& options) {
    WordData words;
    std::string word;
    uint64_t cnt;
    std::ifstream stream(options.src_file);
    while (std::getline(stream, word, '\t')) {
        stream >> cnt;
        words.emplace_back(word, cnt);
        stream.get();
    }

    return words;
}

void RunReduce(const TMapReduceOptions& options) {
    auto words = ScanWords(options);
    std::sort(words.begin(), words.end());
    uint64_t file_num = SplitIntoJobs(options, words);
    MergeOutputs(options, file_num);
    RemoveTempFiles(file_num);
}

void RunMap(const TMapReduceOptions& options) {
    bp::system(options.script_path, bp::std_out > options.dst_file, bp::std_in < options.src_file);
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

enum EMapReduceArgs {
    Type = 1,
    ScriptPath = 2,
    SrcFile = 3,
    DstFile = 4
};

int main(int num_args, char* arg_values[]) {
    if (num_args != 5) {
        throw std::runtime_error("MapReduce must take 5 arguments");
    }

    TMapReduceOptions options;
    options.type = arg_values[EMapReduceArgs::Type];
    options.script_path = arg_values[EMapReduceArgs::ScriptPath];
    options.src_file = arg_values[EMapReduceArgs::SrcFile];
    options.dst_file = arg_values[EMapReduceArgs::DstFile];

    RunMapReduce(options);

    return 0;
}

