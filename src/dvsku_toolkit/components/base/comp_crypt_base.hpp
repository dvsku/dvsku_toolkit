#pragma once

#include "components/base/comp_evp_base.hpp"

namespace dvsku_toolkit {
    class comp_crypt_base : public comp_evp_base {
    public:
        comp_crypt_base(components_bundle& components);
    };
}
