#include "utility/common/guid.h"

namespace CGUID
{
    tString create_uuid()
    {
        uuid_t uu;
        uuid_generate(uu);
        
        uuid_string_t str;
        uuid_unparse_lower(uu ,str);
        
        return str;
    }
    
}
