#include "Tefnout.hpp"

#include "Tefnout/Utility/StreamIO.hpp"
#include "Test/TreeTraits.hpp"
#include <iostream>


class SampleApp : public Tefnout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefnout::Application* Tefnout::CreateApplication()
{
    // {
    //     // Test Traits
    //     using namespace Tree;

    //     BST<SimpleNode, int> bst1;
    //     bst1.Add(42);

    //     BST<NodeWithDepth, int> bst2;
    //     bst2.Add(42);
    // }


    {
        // @TODO Must be in test section
        // Test Stream IO
        using namespace Utility;

        auto content = StreamIO::ReadFile("");
        std::cout << content << std::endl;
    }


    return new SampleApp();
}


