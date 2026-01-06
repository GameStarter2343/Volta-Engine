#include "../headers/Render.h"
#include "../headers/Camera.h"
#include "../headers/Debug.h"

int main() {
    Debug d(true);
    Camera cam;

    Render r(&cam, &d, 1920, 1080, "external/shaders/test");

    while (!r.ShouldClose()) {
        r.Update();

        r.DrawTriangle(); // previously in main
        r.SwapPoll();
    }

    return 0;
}
