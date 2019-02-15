#include "pcap.h"

typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;

}ip_address;

typedef struct mac_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
	u_char byte5;
	u_char byte6;

}mac_address;

typedef struct ip_header
{
	u_char		ver_hrdlen;		// �汾��(4 bits) + �ײ�����(4 bits)
	u_char		tos;			// ��������
	u_short		totallen;		// �ܳ���
	u_short		identifier;	// ��ʶ
	u_short		flags_offset;	// ��־(3 bits) + Ƭƫ��(13 bits)
	u_char		ttl;			// ����ʱ��
	u_char		proto;			// �ϲ�Э��
	u_short		checksum;		// �ײ�У���
	ip_address	srcaddr;		// Դ��ַ
	ip_address	dstaddr;		// Ŀ�ĵ�ַ
	u_int		option_padding;	// ѡ������

}ip_header;


typedef struct arp_header
{
	u_short		hardtype;		// Ӳ������
	u_short		prototype;		// Э������
	u_char		hardlen;		// Ӳ������
	u_char		protolen;		// Э�鳤��
	u_short		op;				// ������
	mac_address	srcmac;			// Դmac��ַ
	ip_address	srcip;			// Դip��ַ
	mac_address	dstmac;			// Ŀ��mac��ַ
	ip_address  dstip;			// Ŀ��ip��ַ

}arp_header;

typedef struct udp_header
{
	u_short srcport;			// Դ�˿�
	u_short dstport;			// Ŀ�Ķ˿�
	u_short	len;				// ����
	u_short checksum;			// У���

}udp_header;

typedef struct tcp_header
{
	u_short		srcport;			// Դ�˿�
	u_short		dstport;			// Ŀ�Ķ˿�
	u_long		seq;				// ���
	u_long		ack;				// ȷ�Ϻ�
	u_short		hdrlen_rsv_flags;	// �ײ�����(4 bits) + ����(6 bits) + URG(1 bit) + ACK(1 bit) + PSH(1 bit) + RST(1 bit) + SYN(1 bit) + FIN(1 bit)
	u_short		win_size;			// ���ڴ�С
	u_short		chksum;				// У���
	u_short		urg_ptr;			// ����ָ��
	u_long		option;				// ѡ��

}tcp_header;

typedef struct dns_header
{
	u_short		identifier;			// ��ʶ
	u_short		flags;				// ��־
	u_short		questions;			// ��ѯ��¼��
	u_short		answers;			// �ش��¼��
	u_short		authority;			// ��Ȩ�ش��¼��
	u_short		additional;			// ������Ϣ��¼��

}dns_header;

typedef struct dns_query
{
	u_short type;					// ��ѯ����
	u_short classes;				// ��ѯ��

}dns_query;

typedef struct dns_answer
{
	u_short type;					// ����
	u_short classes;				// ��
	u_long	ttl;					// ����ʱ��

}dns_answer;

typedef struct icmp_header
{
	u_char	type;					// ����
	u_char	code;					// ����
	u_short chksum;					// У���
	u_long  others;					// �ײ��������֣��ɱ���������ȷ����Ӧ���ݣ�

}icmp_header;

/* chaddr�ֶε�option�ֶ���decodeDHCP�н��� */
typedef struct dhcp_header
{
	u_char	op;						// ��������
	u_char	htype;					// Ӳ������
	u_char	hlen;					// Ӳ����ַ����
	u_char	hops;					// ����
	u_long	xid;					// ����ID
	u_short secs;					// �ͻ�����ʱ��
	u_short flags;					// ��־
	ip_address ciaddr;				// �ͻ���IP��ַ
	ip_address yiaddr;				// ���IP��ַ
	ip_address siaddr;				// ������IP��ַ
	ip_address giaddr;				// ����IP��ַ
//  u_char[16] chaddr;				// �ͻ�Ӳ����ַ
//  u_char[64] sname;				// ������������
//  u_char[128] file;				// �����ļ���
//  options(variable)				// ѡ��䳤��
	
}dhcp_header;

typedef struct packet_header
{
	mac_address		saddr;			// Դmac��ַ
	mac_address		daddr;			// Ŀ��mac��ַ
	u_short			eth_type;		// ��̫��֡�����ֶ�
	ip_header		*iph;			// ip�ײ�
	arp_header		*arph;			// arp�ײ�
	icmp_header     *icmph;			// icmp�ײ�
	udp_header		*udph;			// udp�ײ�
	tcp_header		*tcph;			// tcp�ײ�
	dns_header		*dnsh;			// dns�ײ�
	u_char			*pkt_data;		// �������ݰ�
	int				caplen;			// �������ݰ�����

}packet_header;



