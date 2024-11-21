#pragma once


namespace BFCAD {

    class BFLTException {
    private:
        int err_code;
        const char *err_str;

    public:
        BFLTException(const char *err_str, int err_code);

        virtual const char* what() const;

    };

}