# RayEngine

RayEngine is a minimal **C++ application framework** with a layered architecture, centralized logging, and modern design patterns.  
It is currently in early development and serves as a foundation for building modular, extensible rendering or simulation systems.

---

## ✨ Overview

The engine provides:
- A **global Application** singleton that manages the main loop and layer stack.
- A **Layer system** with lifecycle hooks (`OnAttach`, `OnDetach`, `OnUpdate`) for modular runtime logic.
- A **centralized logging system** wrapping `spdlog` with convenience macros.
- Clear ownership semantics using modern C++ smart pointers and RAII.
- Example projects (`Sandbox`) showcasing direct and asynchronous layer operations.

> RayEngine is not a full renderer yet — it focuses on **architecture, modularity, and clean C++ design** as the foundation for a future path tracer.

---
## 💻 Getting Started

Clone the repository and build the project using CMake:

```bash
git clone https://github.com/<your-username>/RayEngine.git
cd RayEngine
mkdir build && cd build
cmake ..
make
./Sandbox
```

## 🧩 Architecture & Design Patterns

| Pattern | Where | Why |
|----------|--------|-----|
| **Singleton / Monostate** | `Application::GetInstance()` and `RayEngine::Log` | Provides a single global application instance and a centralized logging service. |
| **Facade / Wrapper** | `RayEngine::Log` wrapping `spdlog` | Simplifies logging API, centralizes setup and shutdown. |
| **Template Method** | `Layer` base class (`OnAttach`, `OnDetach`, `OnUpdate`) | Defines lifecycle hooks for derived layers. |
| **Layer Stack / Collection** | `Application::GetInstance().GetLayerStack()` | Manages ordered composition of systems (UI, gameplay, overlays). |
| **RAII / Smart Pointers** | `std::unique_ptr` for layers, `std::shared_ptr` for loggers | Ensures deterministic cleanup and ownership clarity. |
| **Non-Copyable Base** | `Layer` is non-copyable/non-movable | Prevents slicing and accidental copying of polymorphic types. |
| **Convenience Macros** | Logging macros (`RAY_CORE_INFO`, `RAY_CLIENT_WARN`, etc.) | Enables concise and consistent logging across modules. |

---


## 🚀 Future Plans:
 - Add windowing and event system via GLFW.
 - Implement a basic renderer (path tracing prototype).
 - Extend the LayerStack with thread-safe push/pop (command queue).
 - Introduce unit tests and mockable dependencies to reduce singleton coupling.
 - Add performance profiling and frame-time visualization.
