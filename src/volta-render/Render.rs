pub mod buffer;
pub mod pipeline;

use ash::{Device, Instance, vk};
use std::rc::Rc;

pub struct VoltaRenderer {
    device: Rc<Device>,
    instance: Instance,
    // Ссылки на наши модули
    main_pipeline: pipeline::GraphicsPipeline,
    vertex_buffer: buffer::VoltaBuffer,
}

impl VoltaRenderer {
    pub fn draw_frame(&self) {
        unsafe {
            // Команды отрисовки:
            // self.device.cmd_bind_pipeline(...)
            // self.device.cmd_draw(...)
        }
    }
}
