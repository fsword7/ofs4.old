
namespace utils
{
    class DataLoader
    {
        public:
            DataLoader() = default;
            ~DataLoader() = default;

            void loadFile(fs::path file);
            void loadMoreData(fs::path path);

            std::vector<std::string> split(const std::string &line, const std::string &separator);
    };
}