use ash::{Device, Entry, Instance, vk};
use ash_window;
use raw_window_handle::{HasDisplayHandle, HasWindowHandle};
use std::ffi::CStr;

pub struct VoltaRenderer {
    _entry: Entry,
    instance: Instance,
    device: Device,
    physical_device: vk::PhysicalDevice,
    surface: vk::SurfaceKHR,
    surface_loader: ash::khr::surface::Instance,
    queue: vk::Queue,
}

impl VoltaRenderer {
    pub fn new(window: &winit::window::Window) -> Self {
        unsafe {
            let entry = Entry::load().expect("Не удалось загрузить Vulkan");

            // 1. Расширения для работы с окном
            let mut extensions = ash_window::enumerate_required_extensions(
                window.display_handle().unwrap().as_raw(),
            )
            .unwrap()
            .to_vec();

            // Добавляем портативность для macOS (если нужно)
            // extensions.push(ash::khr::portability_enumeration::NAME.as_ptr());

            let app_info = vk::ApplicationInfo::default()
                .application_name(unsafe { CStr::from_bytes_with_nul_unchecked(b"Volta\0") })
                .engine_version(vk::make_api_version(0, 1, 0, 0))
                .api_version(vk::API_VERSION_1_3);

            let create_info = vk::InstanceCreateInfo::default()
                .application_info(&app_info)
                .enabled_extension_names(&extensions);

            let instance = entry
                .create_instance(&create_info, None)
                .expect("Ошибка создания Instance");

            // 2. Создание поверхности (Surface)
            let surface = ash_window::create_surface(
                &entry,
                &instance,
                window.display_handle().unwrap().as_raw(),
                window.window_handle().unwrap().as_raw(),
                None,
            )
            .expect("Ошибка создания Surface");

            let surface_loader = ash::khr::surface::Instance::new(&entry, &instance);

            // 3. Выбор видеокарты (Physical Device)
            let pdevices = instance.enumerate_physical_devices().unwrap();
            let physical_device = pdevices[0]; // Берем первую попавшуюся для примера

            // 4. Логическое устройство и очереди
            let queue_family_index = 0; // В реальном движке нужно искать графическую очередь
            let priorities = [1.0];
            let queue_info = vk::DeviceQueueCreateInfo::default()
                .queue_family_index(queue_family_index)
                .queue_priorities(&priorities);

            let device_extensions = [ash::khr::swapchain::NAME.as_ptr()];

            let device_create_info = vk::DeviceCreateInfo::default()
                .queue_create_infos(std::slice::from_ref(&queue_info))
                .enabled_extension_names(&device_extensions);

            let device = instance
                .create_device(physical_device, &device_create_info, None)
                .expect("Ошибка создания Device");

            let queue = device.get_device_queue(queue_family_index, 0);

            Self {
                _entry: entry,
                instance,
                device,
                physical_device,
                surface,
                surface_loader,
                queue,
            }
        }
    }

    pub fn draw_frame(&self) {
        // Здесь будет логика Swapchain, Command Buffers и Submit
        // На данном этапе просто убеждаемся, что GPU не занят
        unsafe {
            self.device.device_wait_idle().unwrap();
        }
    }
}

// Очистка ресурсов при закрытии
impl Drop for VoltaRenderer {
    fn drop(&mut self) {
        unsafe {
            self.device.destroy_device(None);
            self.surface_loader.destroy_surface(self.surface, None);
            self.instance.destroy_instance(None);
        }
    }
}
