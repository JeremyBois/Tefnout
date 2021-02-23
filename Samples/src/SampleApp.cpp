#include "Tefnout.hpp"
#include "Tefnout/Core/EntryPoint.hpp"

#include "SampleLayer.hpp"
#include "Test/TreeTraits.hpp"

#include <iostream>
#include <memory>
#include <type_traits>

class SampleApp : public Tefnout::Application
{
  public:
    SampleApp()
    {
        // Create user application layer
        GetLayerStack().Push(new SampleLayer());
    }
    ~SampleApp() = default;
};

Tefnout::Application *Tefnout::CreateApplication()
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
        std::cout << content.value_or("Not able to read the file") << std::endl;
    }

    // // @Test Stack
    // {
    //     struct Test
    //     {
    //         int Value;

    //         Test(int val) : Value(val)
    //         {
    //         }

    //         void OnPush()
    //         {
    //             USER_INFO("OnPush {0}", Value);
    //         }
    //         void OnPop()
    //         {
    //         }
    //     };
    //     auto stack = Stack<Test>{};
    //     stack.Push(new Test(1));
    //     stack.Push(new Test(2));
    //     stack.PushLast(new Test(11));
    //     stack.Push(new Test(3));
    //     for (auto &element : stack)
    //     {
    //         USER_INFO("Element is {0}", element->Value);
    //     }

    //     for (auto it = stack.cbegin(); it != stack.cend(); it++)
    //     {
    //         USER_INFO("Const - Element is {0}", (*it)->Value);
    //     }

    //     for (auto it = stack.rbegin(); it != stack.rend(); it++)
    //     {
    //         USER_INFO("Reverse - Element is {0}", (*it)->Value);
    //     }

    //     for (auto it = stack.crbegin(); it != stack.crend(); it++)
    //     {
    //         USER_INFO("Const reverse - Element is {0}", (*it)->Value);
    //     }

    //     class TTTT : public ApplicationLayer
    //     {
    //       public:
    //         TTTT() = default;
    //         virtual ~TTTT() = default;
    //     };
    //     auto test = ApplicationLayer();

    //     auto layerStack = Stack<ApplicationLayer>{};
    //     layerStack.Push(new ApplicationLayer{});
    //     layerStack.Push(new ApplicationLayer{});
    //     layerStack.Push(new ApplicationLayer{});
    //     layerStack.Push(new TTTT{});

    //     // for (const auto& element: layerStack)
    //     // {
    //     //     USER_INFO("Element is {0}", element->Value);
    //     // }
    // }

    return new SampleApp();
}
