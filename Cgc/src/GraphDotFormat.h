/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef GRAPHDOTFORMAT_H
#define GRAPHDOTFORMAT_H
namespace Cgc
{
    /** prints to an ostream in graphviz's "dot" format.
    Given a graph name and the graph, the print method will print the graph to 
    a format readable by graphviz's dotty display.  There are some methods that are set up to allow
    a subclass to better control the formatting in dot.
    @see http://www.graphviz.org for more information on dot format and graphviz.
    */
    template<class NetType>
    class GraphDotFormat
    {
    public:
        /// construct the object with a graph name and the graph
        GraphDotFormat(const std::string &graphName,const NetType &net)
            :mNet(net),
            mGraphName(graphName)
        { }
        /// This method will dump the graph to the stream.
        void print(std::ostream &os)const
        {
            os<<"digraph \""<<getGraphName()<<"\" "<<getGraphAttributes(&mNet)<<" { "<<std::endl;
            typename NetType::const_iterator nodeIt;
            typename NetType::const_arc_iterator arcIt;
            for(nodeIt = mNet.begin();nodeIt != mNet.end();nodeIt++)
            {
                NodeId tailNodeId = mNet.getNodeId(nodeIt);
                os<<"\t\""<<tailNodeId<<"\" "<<getNodeAttributes(&(*nodeIt))<<std::endl;
                for(arcIt = (*nodeIt).begin(); arcIt != (*nodeIt).end(); arcIt++)
                {
                    NodeId headNodeId = mNet.getNodeId((*arcIt).head());
                    os<<"\t\""<<tailNodeId<<"\" -> \""<<headNodeId<<"\" "<<getArcAttributes(&(*arcIt))<<std::endl;
                }
            }
            os<<"}"<<std::endl;
        }
    protected:
        /// overload this method if you want to name your graph differently.
        virtual std::string getGraphName()const { return mGraphName;}
        /// overload this method if you want to better control the node formatting.
        virtual std::string getNodeAttributes(const typename NetType::Node *const node)const
        { return "";}
        /// overload this method if you want to better control the arc formatting.
        virtual std::string getArcAttributes(const typename NetType::Arc *const node)const
        { return "";}
        /// overload this method if you want the graph to have certain attributes.
        virtual std::string getGraphAttributes(const NetType *const node)const
        { return "";}
    private:
        const NetType &mNet;
        std::string mGraphName;
    };
}
#endif
