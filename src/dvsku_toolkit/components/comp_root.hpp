#pragma once

namespace dvsku_toolkit {
    class components_bundle;

    class comp_root {
    public:
        comp_root(components_bundle& components);

    private:
        components_bundle& m_components;
    };
}
