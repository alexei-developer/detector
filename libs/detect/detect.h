#include <string>
#include <exception>

namespace detect {

  class DetectNotFound : public std::exception
  {
    public:
      DetectNotFound(const std::string& message);
      const char* what() const throw();

    private:
      const std::string message_;
  };

  bool init();

  void video_process(const std::string& url);
  void video_process(const int& device);

  std::string DetectFace(const std::string& path_image);
  std::string DetectPedestrian(const std::string& path_image);

}
