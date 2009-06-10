#ifndef PAXOSACCEPTOR_H
#define PAXOSACCEPTOR_H

#include "System/Common.h"
#include "Framework/Transport/TransportWriter.h"
#include "Framework/AsyncDatabase/AsyncDatabase.h"
#include "Framework/Database/Transaction.h"
#include "PaxosMsg.h"
#include "PaxosState.h"

class PaxosAcceptor
{
friend class ReplicatedLog;

public:
	PaxosAcceptor();
	
	void					Init(TransportWriter** writer_);
	bool					Persist(Transaction* transaction);
	bool					IsWriting() { return mdbop.IsActive(); }
	
protected:
	bool					WriteState();
	bool					ReadState();
	void					SendReply(unsigned nodeID);
	void					OnPrepareRequest(PaxosMsg& msg_);
	void					OnProposeRequest(PaxosMsg& msg_);
	void					OnDBComplete();

	TransportWriter**		writers;
	ByteBuffer				wdata;
	uint64_t				paxosID;
	PaxosMsg				msg;
	unsigned				senderID;
	PaxosAcceptorState		state;
	Table*					table;
	Transaction				transaction;
	MultiDatabaseOp			mdbop;
	ByteArray<128>			data[4];
	uint64_t				writtenPaxosID;
	MFunc<PaxosAcceptor>	onDBComplete;
};

#endif
