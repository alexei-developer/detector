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

  std::string DetectFace(const std::string& path_image);
  std::string DetectPedestrian(const std::string& path_image);

}
