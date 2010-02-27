/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef COMPONENT_H
#define COMPONENT_H
#ifndef NODEID_H
#include <NodeId.h>
#endif
#include <set>

namespace Cgc
{
	/**@brief a single "component" is defined by a set of NodeIds.
	@ingroup Algorithms
	This is used to define the types returned to the user.
	@see CompCollection 
	@see ConnComp
	*/
	class Component : public std::set<NodeId> 
	{
	public:
		bool isSubSet(const Component &other)const
		{
			for(Component::const_iterator comp1It = begin();comp1It!=end();comp1It++)
			{
				if(other.find(*comp1It)==other.end())
					return false;
			}
			return true;
		}
	};
	/**@brief a collection of components.
	@ingroup Algorithms
	The main purpose of this class is to add the additional "mergeComponents" method.  
	@see ConnComp
	@see Component
	*/
	class CompCollection:public std::set<Component> 
	{
	public:
		/** @brief This method reduces the number of components.  
		In directed graphs, one may find that some Component s are strict subsets of others.  This removes the subset Component s.
		*/
		bool mergeComponents()
		{
			bool bFoundOne=false;
			// for each component
			bool bRestart=true;
			while(bRestart)
			{
				bRestart=false;
				iterator it1Last = end();
				if(it1Last!=begin())
					it1Last--;
				for(iterator it1 = begin();it1!=it1Last;it1++)
				{
					// for every "later" component
					iterator it2 = it1;
					it2++;
					for(;it2!=end();it2++)
					{
						if((*it1).isSubSet((*it2)))
						{
							// it1 referes to a component which is a subset of it2
							erase(it1);
							bFoundOne=true;
							bRestart=true;
							break;
						}
						else if((*it2).isSubSet((*it1)))
						{
							// it2 referes to a component which is a subset of it1
							erase(it2);
							bFoundOne=true;
							bRestart=true;
							break;
						}
						if(bRestart)
							break;
					}
					if(bRestart)
						break;
				}
			}
			return bFoundOne;
		}
	};

	/** @brief  A print utilitye to assist in using Components.
	@ingroup PublicInterface
	*/
	std::ostream &operator<<(std::ostream &os, const Component &comp)
	{
		os<<" CompSize="<<comp.size()<<std::endl;
		unsigned int cnt=0;
		for(Component::const_iterator cci = comp.begin();
			cci != comp.end();cci++)
		{
			os<<(*cci);
			if(cnt<comp.size()-1)
				os<<" - ";
			cnt++;
		}
		return os<<std::endl;
	}

	/** @brief  A print utilitye to assist in using Components.
	@ingroup PublicInterface
	*/
	std::ostream &operator<<(std::ostream &os,const CompCollection &coll)
	{
		os<<" Component collection has :"<<coll.size()<<" components."<<std::endl;
		for(CompCollection::const_iterator collIt = coll.begin();
			collIt!=coll.end();collIt++)
		{
			os<<(*collIt)<<std::endl;
		}
		return os;
	}
}
#endif

