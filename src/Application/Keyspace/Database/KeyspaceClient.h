#ifndef KEYSPACECLIENT_H
#define KEYSPACECLIENT_H

#include "System/Buffer.h"
#include "KeyspaceDB.h"

class KeyspaceOp;

class KeyspaceClient
{
public:
	virtual			~KeyspaceClient() {}
	virtual	void	OnComplete(KeyspaceOp* op, bool status, bool final = true) = 0;
	
	void Init(KeyspaceDB* kdb_)
	{
		kdb = kdb_;
		numpending = 0;
	}


	bool Add(KeyspaceOp *op)
	{
		bool ret;
		
		ret = kdb->Add(op);
		if (ret)
			numpending++;
		return ret;
	}

protected:
	int				numpending;
	KeyspaceDB*		kdb;
};


class KeyspaceOp
{
public:
	enum Type
	{
		GET,
		DIRTY_GET,
		LIST,
		DIRTY_LIST,
		SET,
		TEST_AND_SET,
		ADD,
		DELETE
	};
	
	Type					type;
	ByteBuffer				key;
	ByteBuffer				value;
	ByteBuffer				test;
	ByteBuffer				prefix;
	int64_t					num;
	uint64_t				count;
	
	KeyspaceClient*			client;
	
	~KeyspaceOp()
	{
		Free();
	}
	
	void Free()
	{
		key.Free();
		value.Free();
		test.Free();
		prefix.Free();
	}
};

#endif
