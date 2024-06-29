#pragma once

namespace dvsku_toolkit {
    class dt_app;

    class dt_system_base {
    public:
        dt_system_base() = delete;
        dt_system_base(dt_app& app) : m_app(app) {}

    protected:
        dt_app& m_app;
    };
}
