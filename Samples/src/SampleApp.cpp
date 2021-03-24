#include "Tefnout.hpp"
#include "Tefnout/Core/EntryPoint.hpp"

#include "SampleLayer.hpp"
#include "Tefnout/Core/Logger.hpp"

#include "Test/TreeTraits.hpp"

// @TEMP
#include "Tefnout/Memory/Allocator/LinearAllocator.hpp"
#include "Tefnout/Memory/BoundsCheckingNull.hpp"
#include "Tefnout/Memory/MemoryBlockHeap.hpp"
#include "Tefnout/Memory/MemoryHandler.hpp"
#include "Tefnout/Memory/MemoryTrackingCounter.hpp"
#include "Tefnout/Memory/MemoryTrackingNull.hpp"
#include "Tefnout/Memory/ThreadHandlerMulti.hpp"
#include "Tefnout/Memory/ThreadHandlerSingle.hpp"

#include <chrono>
// #include <exception>
#include <future>
#include <iostream>
#include <memory>
#include <ratio>
#include <thread>
#include <type_traits>
#include <vector>

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
    // @TEST Traits
    // {
    //     // Test Traits
    //     using namespace Tree;

    //     BST<SimpleNode, int> bst1;
    //     bst1.Add(42);

    //     BST<NodeWithDepth, int> bst2;
    //     bst2.Add(42);
    // }

    // // @TEST Stream
    // {
    //     // @TODO Must be in test section
    //     // Test Stream IO
    //     using namespace Utility;

    //     auto content = StreamIO::ReadFile("");
    //     std::cout << content.value_or("Not able to read the file") << std::endl;
    // }

    // {
    //     p point to memory address 36
    //     long *p = (long *)0x24;
    //     *p = 15;
    //     long *t = (long*)(p+sizeof(long));
    //     long *t = *(p + 1)
    //     *t = 16;

    //     // TEFNOUT_INFO("{0} - {1}", *p, *t);

    // }

    // {
    //     // Exception en héritant de l'interface fournit par le C++
    //     class MyException : public std::exception
    //     {
    //       public:
    //         // Can be used when catched
    //         char *what()
    //         {
    //             return "MyException raised !";
    //         }
    //     };

    //     try
    //     {
    //         throw MyException();
    //     }
    //     catch (MyException &e)
    //     {
    //         std::cout << e.what() << std::endl;
    //     }

    //     // Exception sans héritage
    //     class AnotherException {};

    //     try
    //     {
    //         throw AnotherException();
    //     }
    //     catch (AnotherException &e)
    //     {
    //         std::cout << "AnotherException raised !" << std::endl;
    //     }
    // }

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

    // // @TEST MemoryHandler
    // {
    //     using namespace Tefnout::Memory;

    //     // typedef Handler<LinearAllocator, ThreadHandlerSingle, BoundsCheckingNull,
    //     //                 MemoryTrackingCounter>
    //     //     TestMemoryHandler;

    //     typedef Handler<LinearAllocator, ThreadHandlerSpinLock, BoundsCheckingNull,
    //                     MemoryTrackingCounter>
    //         TestMemoryHandler;

    //     MemoryBlockHeap heapArea(1024);
    //     auto memoryHandler = TestMemoryHandler(heapArea);

    //     struct Couscous
    //     {
    //       public:
    //         int aInt = 5;
    //         int aFloat = 10.0f;
    //     };

    //     // Create
    //     std::vector<std::future<void>> promises;
    //     promises.reserve(20);

    //     auto *aFloatPtr = MakeNew<float>(memoryHandler, "Test");
    //     USER_INFO("Float {0}", *aFloatPtr);

    //     promises.emplace_back(std::async(std::launch::async, [&memoryHandler]() -> void {
    //         std::array<Couscous *, 10> couscous;

    //         for (int i = 0; i < 10; ++i)
    //         {
    //             couscous[i] = MakeNew<Couscous>(memoryHandler, "Miaous");
    //             couscous[i]->aInt = i;
    //             couscous[i]->aFloat = i;
    //         }

    //         for (auto c : couscous)
    //         {
    //             USER_INFO("Couscous {0} - {1}", c->aInt, c->aFloat);
    //             MakeDelete(c, memoryHandler);
    //         }
    //     }));

    //     auto thread = std::thread([&memoryHandler](){
    //         std::array<Couscous *, 10> couscous;

    //         for (int i = 0; i < 10; ++i)
    //         {
    //             couscous[i] = MakeNew<Couscous>(memoryHandler, "Miaous");
    //             couscous[i]->aInt = i * -10;
    //             couscous[i]->aFloat = i * -10;
    //         }

    //         for (auto c : couscous)
    //         {
    //             USER_INFO("Couscous {0} - {1}", c->aInt, c->aFloat);
    //             MakeDelete(c, memoryHandler);
    //         }
    //     });

    //     auto thread2 = std::thread([&memoryHandler](){
    //         std::array<Couscous *, 10> couscous;

    //         for (int i = 0; i < 10; ++i)
    //         {
    //             couscous[i] = MakeNew<Couscous>(memoryHandler, "Miaous");
    //             couscous[i]->aInt = i * -100;
    //             couscous[i]->aFloat = i * -100;
    //         }

    //         for (auto c : couscous)
    //         {
    //             USER_INFO("Couscous {0} - {1}", c->aInt, c->aFloat);
    //             MakeDelete(c, memoryHandler);
    //         }
    //     });

    //     promises.emplace_back(std::async(std::launch::async, [&memoryHandler]() -> void {
    //         std::array<Couscous *, 10> couscous;

    //         for (int i = 0; i < 10; ++i)
    //         {
    //             couscous[i] = MakeNew<Couscous>(memoryHandler, "Miaous");
    //             couscous[i]->aInt = i * 10;
    //             couscous[i]->aFloat = i * 10;
    //         }

    //         for (auto c : couscous)
    //         {
    //             USER_INFO("Couscous {0} - {1}", c->aInt, c->aFloat);
    //             MakeDelete(c, memoryHandler);
    //         }
    //     }));

    //     promises.emplace_back(std::async(std::launch::async, [&memoryHandler]() -> void {
    //         std::array<Couscous *, 10> couscous;

    //         for (int i = 0; i < 10; ++i)
    //         {
    //             couscous[i] = MakeNew<Couscous>(memoryHandler, "Miaous");
    //             couscous[i]->aInt = i * 100;
    //             couscous[i]->aFloat = i * 100;
    //         }

    //         for (auto c : couscous)
    //         {
    //             USER_INFO("Couscous {0} - {1}", c->aInt, c->aFloat);
    //             MakeDelete(c, memoryHandler);
    //         }
    //     }));

    //     auto *couscous = MakeNew<Couscous>(memoryHandler, "Test");
    //     couscous->aInt = 33;
    //     couscous->aFloat = 11.0;
    //     USER_INFO("Couscous {0} - {1}", couscous->aInt, couscous->aFloat);

    //     promises.emplace_back(std::async(std::launch::async, [&memoryHandler]() -> void {
    //         auto *couscous = MakeNew<Couscous>(memoryHandler, "Miaous");
    //         couscous->aInt = 3;
    //         couscous->aFloat = 3.0;
    //         USER_INFO("Couscous {0} - {1}", couscous->aInt, couscous->aFloat);
    //         MakeDelete(couscous, memoryHandler);
    //     }));

    //     auto *anotherAnotherCouscous = MakeNew<Couscous>(memoryHandler, "Test");
    //     anotherAnotherCouscous->aFloat = 1001.0;
    //     USER_INFO("Couscous {0} - {1}", anotherAnotherCouscous->aInt,
    //               anotherAnotherCouscous->aFloat);

    //     // Clean
    //     MakeDelete(couscous, memoryHandler);
    //     MakeDelete(anotherAnotherCouscous, memoryHandler);
    //     MakeDelete(aFloatPtr, memoryHandler);

    //     // Wait
    //     for (auto &promise : promises)
    //     {
    //         promise.get();
    //     }

    //     thread.join();
    //     thread2.join();
    // }

    return new SampleApp();
}
