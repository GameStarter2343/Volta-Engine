use winit::{
    application::ApplicationHandler,
    event::{ElementState, WindowEvent},
    event_loop::{ActiveEventLoop, EventLoop},
    keyboard::{KeyCode, PhysicalKey},
    window::{Fullscreen, Window, WindowAttributes, WindowId},
};

// Импортируем ваш рендерер
use volta_render::VoltaRenderer;

struct App {
    window: Option<Window>,
    renderer: Option<VoltaRenderer>, // Храним тут
}

impl ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let monitor = event_loop.primary_monitor();
        let attrs = WindowAttributes::default()
            .with_title("Volta Engine")
            .with_fullscreen(Some(Fullscreen::Borderless(monitor)));

        let window = event_loop.create_window(attrs).unwrap();

        // Инициализируем рендерер сразу после создания окна
        // Передаем ссылку на окно для создания Surface
        let renderer = VoltaRenderer::new(&window);

        self.window = Some(window);
        self.renderer = Some(renderer);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),

            WindowEvent::KeyboardInput { event, .. } => {
                if event.state == ElementState::Pressed {
                    if let PhysicalKey::Code(KeyCode::Escape) = event.physical_key {
                        event_loop.exit();
                    }
                }
            }

            // Главный цикл отрисовки
            WindowEvent::RedrawRequested => {
                if let (Some(renderer), Some(window)) = (&self.renderer, &self.window) {
                    renderer.draw_frame();
                    window.request_redraw(); // Запрашиваем следующий кадр (60+ FPS)
                }
            }

            _ => {}
        }
    }
}

fn main() {
    let event_loop = EventLoop::new().unwrap();
    // Инициализируем с None
    let mut app = App {
        window: None,
        renderer: None,
    };
    event_loop.run_app(&mut app).unwrap();
}
