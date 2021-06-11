
namespace utils
{
    class DataLoader
    {
        public:
            DataLoader() = default;
            ~DataLoader() = default;

            void loadFile(fs::path file);
            void loadMoreData(fs::path path);
    };
}