use ash::vk;
use std::rc::Rc;

pub struct VoltaBuffer {
    pub raw: vk::Buffer,
    pub memory: vk::DeviceMemory,
    device: Rc<ash::Device>, // Используем Rc, чтобы буфер "держал" устройство
}

impl VoltaBuffer {
    pub fn new(device: Rc<ash::Device>, size: u64, usage: vk::BufferUsageFlags) -> Self {
        // ... здесь логика vkCreateBuffer и выделение памяти ...
        // Для реального проекта используйте бибилотеку 'gpu-allocator'
        todo!("Реализация создания буфера")
    }
}

// Автоматическая очистка при удалении объекта
impl Drop for VoltaBuffer {
    fn drop(&mut self) {
        unsafe {
            self.device.destroy_buffer(self.raw, None);
            self.device.free_memory(self.memory, None);
        }
    }
}
