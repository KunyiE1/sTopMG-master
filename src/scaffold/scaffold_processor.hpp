//
// Created by kunyili on 9/20/24.
//

#ifndef TOPPIC_SCAFFOLD_PROCESSOR_HPP
#define TOPPIC_SCAFFOLD_PROCESSOR_HPP

#include "scaffold/scaffold_mng.hpp"


namespace toppic {

    class ScaffoldProcessor {
    public:
        ScaffoldProcessor(ScaffoldMngPtr mng_ptr): mng_ptr_(mng_ptr) {};

        void process();

    private:
        ScaffoldMngPtr mng_ptr_;
    };
    typedef std::shared_ptr<ScaffoldProcessor> ScaffoldProcessorPtr;
} // toppic

#endif //TOPPIC_SCAFFOLD_PROCESSOR_HPP
