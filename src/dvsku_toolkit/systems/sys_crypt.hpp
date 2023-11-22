#pragma once

namespace dvsku_toolkit {
    class systems_bundle;

    class sys_crypt {
    public:
        sys_crypt() = delete;
        sys_crypt(systems_bundle& systems);
    
    private:
        systems_bundle& m_systems;
    };
}
