#pragma once

#define MAX_LABELNAME_SIZE	32

// Request Types...
#define AGS_MSGTYPE_REGISTER			1
#define AGS_MSGTYPE_WRITETOSERVER		2
#define AGS_MSGTYPE_WRITEARRAYTOSERVER	3
#define AGS_MSGTYPE_WRITETOCLIENT		4

// Data Types...
#define AGS_DATATYPE_SINT8	1	// INT1
#define AGS_DATATYPE_UINT8	2	// LOG1
#define AGS_DATATYPE_SINT16	3	// INT2
#define AGS_DATATYPE_UINT16	4	// LOG2
#define AGS_DATATYPE_SINT32	5	// INT4
#define AGS_DATATYPE_UINT32	6	// LOG4
#define AGS_DATATYPE_REAL4	7	// REAL4
#define AGS_DATATYPE_REAL8	8	// REAL8

// Union to hold data so we can send multiple data types under one type...
union AGS_LABEL_VALUE
{
	char			s8;		// INT1
	unsigned char	u8;		// LOG1
	short			s16;	// INT2
	unsigned short	u16;	// LOG2
	int				s32;	// INT4
	unsigned int	u32;	// LOG4
	float			f32;	// REAL4
	double			f64;	// REAL8
};

// Type to store a Label Name and the data & value associated with that label...
struct AGS_DATA_RECORD
{
	char label_name[MAX_LABELNAME_SIZE];
	char data_type;
	AGS_LABEL_VALUE data;
};

// Packet used to send/receive by both client and server...
struct AGS_DATA_PACKET
{
	int header;
	char message_type;
	int num_records;
	AGS_DATA_RECORD records[1];
};

// Register types used by client. Used to let the server know what labels a client is interested in...
struct AGS_REGISTER_RECORD
{
	char label_name[MAX_LABELNAME_SIZE];
};

// Packet used to register all the labels the client is interest in as well as the rate it wishes to be updated by...
struct AGS_REGISTER_PACKET
{
	int header;
	char message_type;
	int num_records;
	int update_rate;
	AGS_REGISTER_RECORD records[1];
};