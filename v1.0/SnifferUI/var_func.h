#include "pcap.h"
#include "remote-ext.h"
#include "Afxtempl.h"

#define PCAP_ERRBUFF_SIZE	50


/* ���ļ��ļ��� */
char filename[50];

/* ȫ�ֱ����ؼ� */
CListCtrl *pList1;
CComboBox *pDevList;
CWnd *pStart;
CWnd *pStop;
CTreeCtrl *pTree;


/* ȫ�ֱ���������豸��Ϣ */
pcap_if_t *alldevs,*d;

/* ȫ�ֱ���errbuf����Ŵ�����Ϣ */
char errbuf[PCAP_ERRBUF_SIZE];

/* ȫ�ֱ���adhandle */
pcap_t *adhandle;

/* �豸�б����У���� */
int list_rows = -1;
int list_cols = 0;
int list_count = 0;

/* �������汨�ĵ��ײ� */
CList<packet_header, packet_header> linklist;

/* �̴߳����� */
UINT capture_thread(LPVOID pParam);

/* �������� */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data);

/* �洢�����ײ�	*/
void saveFrame(const u_char *pkt_data, int offset);		//ok
void saveIP(const u_char *pkt_data, int offset);		//ok
void saveARP(const u_char *pkt_data, int offset);		//ok
void saveUDP(const u_char *pkt_data, int offset);		//ok
void saveTCP(const u_char *pkt_data, int offset);		//ok
void saveICMP(const u_char *pkt_data,int offset);		//ok
void saveDNS(const u_char *pkt_data,int offset);		//ok


/* ���������ײ� */
void decodeFrame(mac_address *saddr, mac_address *daddr, u_short *eth_type, HTREEITEM *hParent);
void decodeIP(ip_header *iph,HTREEITEM *hParent);
void decodeARP(arp_header *arph, HTREEITEM *hParent);
void decodeUDP(udp_header *udph, HTREEITEM *hParent);
void decodeTCP(tcp_header *tcph, HTREEITEM *hParent);
void decodeDNS(u_char *pkt_data, int offset, dns_header *dnsh, HTREEITEM *hParent);			// offsetΪ��dns�ײ���ƫ����
void decodeHTTP(u_char *pkt_data, int offset, HTREEITEM *hParent);							// offsetΪ��HTTP���ĵ�ƫ����
void decodeICMP(icmp_header *icmph, HTREEITEM *hParent);		
void decodeDHCP(u_char *pkt_data, int offset, HTREEITEM *hParent);							// offsetΪ��DHCP���ĵ�ƫ����


/* ����ת�� ���涨��ʽ��name2ת��Ϊ����name1 */
void translateName(char *name1, const char *name2);

/* DNS��Դ��¼���ݲ���ת�� ������ָ��c0�ĵ�ַdata2ת��Ϊ��ַdata1 offsetΪ��dns�ײ���ƫ����*/
void translateData(u_char *pkt_data, int offset, char *data1, char *data2, int data2_len);

/* �ж�data������ָ��0xc0,������ָ����data�е�λ��*/
int isNamePtr(char *data);
