use ash::vk;
use std::rc::Rc;

pub struct GraphicsPipeline {
    pub layout: vk::PipelineLayout,
    pub handle: vk::Pipeline,
    device: Rc<ash::Device>,
}

impl GraphicsPipeline {
    pub fn new(device: Rc<ash::Device>, vertex_shader: &str, fragment_shader: &str) -> Self {
        // ... логика загрузки SPIR-V и создания Pipeline ...
        todo!("Сборка графического конвейера")
    }
}

impl Drop for GraphicsPipeline {
    fn drop(&mut self) {
        unsafe {
            self.device.destroy_pipeline(self.handle, None);
            self.device.destroy_pipeline_layout(self.layout, None);
        }
    }
}
