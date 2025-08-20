#include "FrameGraph.h"

#include <vsg/commands/PipelineBarrier.h>

#include <algorithm>
#include <deque>
#include <set>

namespace taal
{
    std::vector<vsg::ref_ptr<Resource>> Processor::inputResources()
    {
        std::vector<vsg::ref_ptr<Resource>> result;
        for (const auto& resource : inputs)
        {
            result.push_back(resource);
        }
        return result;
    }

    std::vector<vsg::ref_ptr<FrameGraphNode>> Processor::outputResources()
    {
        std::vector<vsg::ref_ptr<Resource>> result;
        for (const auto& resource : outputs)
        {
            result.push_back(resource);
        }
        return result;
    }

    vsg::ref_ptr<vsg::Node> Processor::nodeForCommandGraph()
    {
        return vsgNode;
    }


    FrameGraph::FrameGraph() :
        presentable(PresentableSurface::create())
    {
    }

    std::vector<vsg::ref_ptr<FrameGraphNode>>
    FrameGraph::orderGraph(vsg::ref_ptr<PresentableSurface> presentableSurface)
    {
        std::deque<<vsg::ref_ptr<Resource>> currentInputs = {presentableSurface};
        std::vector<vsg::ref_ptr<FrameGraphNode>> result;
        for (; !currentInputs.empty(); currentInputs.pop_front())
        {
            
            if (vsg::ref_ptr<FrameGraphNode> ref_writer = currentInputs.front().writer)
            {
                result.push_back(ref_writer);
                auto inputResources = ref_writer->inputResources();
                for (auto& resource : inputResources)
                {
                    currentInputs.push_back(resource);
                }
            }
        }
        reverse(result.begin(), result.end());
        std::set<vsg::ref_ptr<FrameGraphNode>> nodesSeen;
        for (auto& node : result)
        {
            auto [itr, notSeen] = nodesSeen.insert(node);
            if (!notSeen)
            {
                node = {};
            }
        }
        auto endUniqueItr = std::stable_partition(result.begin(), result.end(),
                                                  [](auto&& refPtr)
                                                  {
                                                      refPtr.valid();
                                                  });
        result.erase(endUniqueItr, result.end());
        return result;
    }

    // XXX Creating a single command graph for now, on the graphics queue.
    vsg::CommandGraphs FrameGraph::createCommandGraphs()
    {

        auto nodes = orderGraph(presentable);
        std::vector<NodeTransitions> nodeTransitions;
        for (auto& node :nodes)
        {
            nodeTransitions.push_back(node->getNodeTransitions());
            
        }
        auto graphicsCommandGraph = vsg::CommandGraph::create();
        for (const auto& node : nodes)
        {
            
        }
    }
}
