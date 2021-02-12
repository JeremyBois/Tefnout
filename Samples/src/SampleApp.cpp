#include "Tefnout.hpp"

#include "Test/TreeTraits.hpp"


class SampleApp : public Tefnout::Application
{
public:
    SampleApp() = default;
    ~SampleApp() = default;
};

Tefnout::Application* Tefnout::CreateApplication()
{
    {
        // Test Traits
        using namespace Tree;

        BST<SimpleNode, int> bst1;
        bst1.Add(42);

        BST<NodeWithDepth, int> bst2;
        bst2.Add(42);
    }


    return new SampleApp();
}


