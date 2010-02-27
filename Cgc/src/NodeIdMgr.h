/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef NODEIDMGR_H
#define NODEIDMGR_H

#include <NodeId.h>
namespace Cgc
{
	/** @brief Manager for node ids used by DynNet
	@ingroup InternalUse
	@see DynNet
	*/
	template <class NetType>
	class NodeIdMgr
	{
		NodeId lastId;

	public:
		// didn't want these two public....but MSVC won't let the friend statement work!
		NodeIdMgr(const NodeId id=NodeId(0)):lastId(NodeId(id)){} 
		NodeId getNewId(){ NodeId retVal=lastId;lastId++;return retVal;}
		void setMaxId(const NodeId &maxId){ lastId=maxId;}
		//friend class NetType;
	};
}
#endif 
