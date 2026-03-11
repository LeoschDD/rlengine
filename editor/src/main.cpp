#include <rlengine.hpp>

class EditorApplication : public rle::Application
{

};

int main()
{
    auto editor_app = std::make_unique<EditorApplication>();
    editor_app->Run();
}