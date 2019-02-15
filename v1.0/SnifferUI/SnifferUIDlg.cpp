// SnifferUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SnifferUI.h"
#include "SnifferUIDlg.h"

#include "packet_header.h"
#include "var_func.h"
#include "string.h"
#include "ctype.h"
//#include "remote-ext.h"

//#include "winsock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnifferUIDlg dialog

CSnifferUIDlg::CSnifferUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnifferUIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSnifferUIDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnifferUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSnifferUIDlg)
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Control(pDX, IDC_LIST1, m_listctl1);
	DDX_Control(pDX, IDC_COMBO1, m_devlist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSnifferUIDlg, CDialog)
	//{{AFX_MSG_MAP(CSnifferUIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSnifferUIDlg message handlers

BOOL CSnifferUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	/* ��ʼ�� */
	pList1 = &m_listctl1;											  // ����б�ؼ���ָ��
	pTree = &m_tree;

	DWORD dwStyle = m_listctl1.GetExtendedStyle();                    // ����б���������
    dwStyle |= LVS_EX_FULLROWSELECT;            
    dwStyle |= LVS_EX_GRIDLINES;                
    m_listctl1.SetExtendedStyle(dwStyle);
	/* �豸�б����	*/
	m_listctl1.InsertColumn(0,"���",LVCFMT_LEFT,40);
	m_listctl1.InsertColumn(1,"ʱ��",LVCFMT_LEFT,180);
	m_listctl1.InsertColumn(2,"����",LVCFMT_LEFT,50);
	m_listctl1.InsertColumn(3,"ԴMAC��ַ",LVCFMT_LEFT,180);
	m_listctl1.InsertColumn(4,"Ŀ��MAC��ַ",LVCFMT_LEFT,180);
	m_listctl1.InsertColumn(5,"ԴIP��ַ",LVCFMT_LEFT,120);
	m_listctl1.InsertColumn(6,"Ŀ��IP��ַ",LVCFMT_LEFT,120);
	m_listctl1.InsertColumn(7,"Э��",LVCFMT_LEFT,50);

	
	/*
	*	��ȡ�豸�б�
	*/

    /* ��ȡ���ػ����豸�б� */
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

	/* ��ӡ�豸�������б���� */
    for(d= alldevs; d != NULL; d= d->next)
    {
        if (d->description)
			
			m_devlist.AddString(d->description);
    }
	
	/* ���� */
	/*
	link = (pkth_linklist*)malloc(sizeof(pkth_linklist));
	link->next = NULL;
	pTail = link;
	*/





	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSnifferUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSnifferUIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSnifferUIDlg::OnQueryDragIcon()
{

	return (HCURSOR) m_hIcon;
}


void CSnifferUIDlg::OnStop() 
{
	// TODO: Add your control notification handler code here

	pStart = GetDlgItem(IDC_START);
	pStop = GetDlgItem(IDC_STOP);

	pStop->EnableWindow(FALSE);
	pStart->EnableWindow(TRUE);

	/* ֹͣץ�� */
	pcap_breakloop(adhandle);
	
}

void CSnifferUIDlg::OnStart() 
{
	// TODO: Add your control notification handler code here

	pStart = GetDlgItem(IDC_START);
	pStop = GetDlgItem(IDC_STOP);
	pDevList = (CComboBox*)GetDlgItem(IDC_COMBO1);
	int sel_index = pDevList->GetCurSel();			//��ȡѡ���豸������
	if (sel_index == CB_ERR)
	{
		AfxMessageBox(_T("û��ѡ����"), MB_OK);
		return;
	}

	pStart->EnableWindow(FALSE);
	pStop->EnableWindow(TRUE);
	
	/* �����߳� */
	myWinThread = AfxBeginThread(capture_thread, NULL, 0, NULL, 0, NULL);
}




/* �̴߳����� */
UINT capture_thread(LPVOID pParam)
{
	/* ��ȡѡ�е��豸 */

	HWND mHwnd = AfxGetMainWnd()->m_hWnd;
	if(mHwnd == NULL)
	{
		AfxMessageBox(_T("��ȡ���ھ��ʧ��"),MB_OK);
		return -1;
	}

	int sel_index = pDevList->GetCurSel();			//��ȡѡ���豸������
	if(sel_index == CB_ERR)
	{
		AfxMessageBox(_T("û��ѡ����"),MB_OK);
		return -1;
	}
		
	int count = 0;
    for(d= alldevs; count < sel_index; d = d->next,count++);


	/* ��ָ���豸 */
	if((adhandle = pcap_open_live(d->name,
					65535,
					 PCAP_OPENFLAG_PROMISCUOUS,
					1000,
					errbuf)) == NULL)
	{ 
		AfxMessageBox(_T("pcap_open_live����!"), MB_OK);
	}

	/* �жϽӿڵ���·�������Ƿ�Ϊ��̫��*/
	if( pcap_datalink(adhandle) != DLT_EN10MB)
		AfxMessageBox(_T("������·�㲻����̫��"), MB_OK);

	
	pcap_dumper_t *dumpfile = NULL;	
	/* �򿪶��ļ�
	strcpy(filename, "pkt_cap");

	dumpfile = pcap_dump_open(adhandle, filename);
	*/

	/*	��ʼ�������ݰ� */
//	AfxMessageBox(_T("��ʼץ��"), MB_OK);
	pcap_loop(adhandle, -1,	packet_handler, (unsigned char *)dumpfile);
//	AfxMessageBox(_T("����ץ��"), MB_OK);
	
	return 0;

}


/* �������� */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	/* д����ļ� */
//	pcap_dump(dumpfile, header, pkt_data);


	/* ��־�ļ� */
	
	char *path = "pkt_cap_log.txt";
	FILE *save_file;
	save_file = fopen(path,"a");	//��׷����ʽд��
	

	/* ��� */
	char str_count[4]; 
	itoa(++list_count, str_count, 10);								// �����������ת��Ϊ�ַ���
	pList1->InsertItem(++list_rows, str_count);

	
	fprintf(save_file, "%d ", list_count);
	fprintf(save_file, " / ");
	
	
	/* ʱ�� */
	//header->ts.
	char * pkt_time = ctime((time_t*)&((header->ts).tv_sec));
	pList1->SetItemText(list_rows, ++list_cols, pkt_time);

	
	fprintf(save_file, "%s", pkt_time);
	fprintf(save_file, " / ");
	

	/* ���� */
	char str_caplen[6];
	itoa(header->caplen, str_caplen, 10);
	pList1->SetItemText(list_rows, ++list_cols, str_caplen);

	
	fprintf(save_file, "len: ");
	fprintf(save_file, "%s ", str_caplen);
	fprintf(save_file, " / ");
	

	/* Դmac*/
	mac_address *src_mac = (mac_address*)(pkt_data + 6);
	/* ����������� */
	CString str_srcmac;
	str_srcmac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), src_mac->byte1, src_mac->byte2, src_mac->byte3, src_mac->byte4, src_mac->byte5, src_mac->byte6);
	pList1->SetItemText(list_rows, ++list_cols, str_srcmac);



	/* �������־�ļ���  */
	
	fprintf(save_file, "src_mac: ");
	fprintf(save_file,"%s", str_srcmac);
	fprintf(save_file, " / ");
	


	/* Ŀ��mac */

	mac_address *dst_mac = (mac_address*)(pkt_data);
		/* ����������� */
	CString str_dstmac;
	str_dstmac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), dst_mac->byte1, dst_mac->byte2, dst_mac->byte3, dst_mac->byte4, dst_mac->byte5, dst_mac->byte6);
	pList1->SetItemText(list_rows, ++list_cols, str_dstmac); 

		/* �������־�ļ��� */
	
	fprintf(save_file, "dst_mac: ");
	fprintf(save_file,"%s", str_dstmac);
	fprintf(save_file, " / ");
	

		


	/* ԴĿIP */
	if(ntohs(*(u_short*)(pkt_data + 12)) == 0x0800)
	{
		ip_header *ip_hdr = (ip_header*)(pkt_data + 14);
		ip_address *src_ip = &ip_hdr->srcaddr;
		ip_address *dst_ip = &ip_hdr->dstaddr;

		
		/* ����������� */
		CString str_srcip;
		str_srcip.Format(_T("%d.%d.%d.%d"), src_ip->byte1, src_ip->byte2, src_ip->byte3, src_ip->byte4);
		pList1->SetItemText(list_rows, ++list_cols, str_srcip);
	
		CString str_dstip;
		str_dstip.Format(_T("%d.%d.%d.%d"), dst_ip->byte1, dst_ip->byte2, dst_ip->byte3, dst_ip->byte4);
		pList1->SetItemText(list_rows, ++list_cols, str_dstip);

		/* �������־�ļ��� */
		
		fprintf(save_file, "src_ip: ");
		fprintf(save_file, str_srcip);
		fprintf(save_file, " / ");
		
		fprintf(save_file, "dst_ip: ");
		fprintf(save_file, str_dstip);
		fprintf(save_file, "\r\n");
		
	}
	else if(ntohs(*(u_short*)(pkt_data + 12)) == 0x0806)
	{
		arp_header *arp_hdr = (arp_header*)(pkt_data + 14);
		ip_address *src_ip = &arp_hdr->srcip;
		ip_address *dst_ip = &arp_hdr->dstip;

		/* ����������� */
		CString str_srcip;
		str_srcip.Format(_T("%d.%d.%d.%d"), src_ip->byte1, src_ip->byte2, src_ip->byte3, src_ip->byte4);
		pList1->SetItemText(list_rows, ++list_cols, str_srcip);
	
		CString str_dstip;
		str_dstip.Format(_T("%d.%d.%d.%d"), dst_ip->byte1, dst_ip->byte2, dst_ip->byte3, dst_ip->byte4);
		pList1->SetItemText(list_rows, ++list_cols, str_dstip);

		/* �������־�ļ��� */
		
		fprintf(save_file, "src_ip: ");
		fprintf(save_file, str_srcip);
		fprintf(save_file, " / ");
		
		fprintf(save_file, "dst_ip: ");
		fprintf(save_file, str_dstip);
		fprintf(save_file, "\r\n");

	}


	/* �洢�������ݰ� */
	packet_header pkth;

	/* ��ʼ�� */
	pkth.arph = NULL;
	pkth.iph  = NULL;
	pkth.icmph = NULL;
	pkth.udph = NULL;
	pkth.tcph = NULL;
	pkth.dnsh = NULL;

	u_char *pkt_data1 = (u_char*)malloc(header->caplen);

	memcpy(pkt_data1, pkt_data, header->caplen);	

	pkth.pkt_data = pkt_data1;
	pkth.caplen = header->caplen;

	linklist.AddTail(pkth);
	


	/* �洢��̫��֡ */
	saveFrame(pkt_data, 0);


	fclose(save_file);

	/* �и�λ */
	list_cols = 0;

}	

/* �洢��̫��֡ */
void saveFrame(const u_char *pkt_data, int offset)
{
	/* ��ȡ��̫��֡�������ֶΡ�ԴĿMAC��ַ */
	u_short eth_type = ntohs(*(u_short*)(pkt_data + 12));
	mac_address *src_mac = (mac_address*)(pkt_data + 6);
	mac_address *dst_mac = (mac_address*)(pkt_data);
	
	/* �����͡�ԴĿMAC��ַ���浽�����β����� */
	linklist.GetTail().saddr = *src_mac;
	linklist.GetTail().daddr = *dst_mac;
	linklist.GetTail().eth_type = eth_type;
	
	/* ������̫��֡�������ֶδ洢���� */
	switch(eth_type)
	{
	case 0x0800: saveIP(pkt_data, 14); 
					break;
	case 0x0806: saveARP(pkt_data, 14); 
					pList1->SetItemText(list_rows, ++list_cols, "ARP"); 
					break;
	default: break;
	}
	
}

/* �洢IP�� */
void saveIP(const u_char *pkt_data, int offset)				//offsetΪip�ײ�����pkt_data��ƫ����
{
	ip_header *ip_hdr = (ip_header*)(pkt_data + offset);

	/* �洢ip�ײ��������� */
	ip_header *p ;

	p = (ip_header*)malloc(sizeof(ip_header));
	p->ver_hrdlen = ip_hdr->ver_hrdlen;
	p->tos = ip_hdr->tos;
	p->totallen = ip_hdr->totallen;
	p->identifier = ip_hdr->identifier;
	p->flags_offset = ip_hdr->flags_offset;
	p->ttl = ip_hdr->ttl;
	p->proto = ip_hdr->proto;
	p->checksum = ip_hdr->checksum;
	p->option_padding = ip_hdr->option_padding;
	p->srcaddr = ip_hdr->srcaddr;
	p->dstaddr = ip_hdr->dstaddr;

	linklist.GetTail().iph = p;

	/* �����ϲ�Э��洢�����ײ� */
	switch(ip_hdr->proto)
	{
	case 1:		saveICMP(pkt_data, offset + (ip_hdr->ver_hrdlen & 0x0f) * 4);
				pList1->SetItemText(list_rows, ++list_cols, "ICMP"); 
				break;	//ICMP

	case 6:		saveTCP(pkt_data, offset + (ip_hdr->ver_hrdlen & 0x0f) * 4); 
				break;	//TCP

	case 17:	saveUDP(pkt_data, offset + (ip_hdr->ver_hrdlen & 0x0f) * 4); 
				break;	//UDP

	default:	pList1->SetItemText(list_rows, ++list_cols, "IPv4"); 
				break;
	}
}


/* �洢ARP�� */
void saveARP(const u_char *pkt_data, int offset)				//offsetΪARP�ײ�����pkt_data��ƫ����
{
	arp_header *arp_hdr = (arp_header*)(pkt_data + offset);

	/* �洢arp�ײ��������� */
	arp_header *p;

	p = (arp_header*)malloc(sizeof(arp_header));
	p->hardtype = arp_hdr->hardtype;
	p->prototype = arp_hdr->prototype;
	p->hardlen = arp_hdr->hardlen;
	p->protolen = arp_hdr->protolen;
	p->op = arp_hdr->op;
	p->srcmac = arp_hdr->srcmac;
	p->srcip = arp_hdr->srcip;
	p->dstmac = arp_hdr->dstmac;
	p->dstip = arp_hdr->dstip;

	linklist.GetTail().arph = p;
	
}

/* �洢ICMP��*/
void saveICMP(const u_char *pkt_data,int offset)
{
	icmp_header *icmp_hdr = (icmp_header*)(pkt_data + offset);

	/* �洢icmp�ײ���������*/
	icmp_header *p;

	p = (icmp_header*)malloc(sizeof(icmp_header));
	p->type = icmp_hdr->type;
	p->code = icmp_hdr->code;
	p->chksum = icmp_hdr->chksum;
	p->others = icmp_hdr->others;

	linklist.GetTail().icmph = p;
}





/* �洢UDP�� */
void saveUDP(const u_char *pkt_data, int offset)				//offsetΪUDP�ײ�����pkt_data��ƫ����
{
	udp_header *udp_hdr = (udp_header*)(pkt_data + offset);

	/* �洢udp�ײ��������� */
	udp_header* p;

	p = (udp_header*)malloc(sizeof(udp_header));
	p->srcport = udp_hdr->srcport;
	p->dstport = udp_hdr->dstport;
	p->len = udp_hdr->len;
	p->checksum = udp_hdr->checksum;

	linklist.GetTail().udph = p;

	/* ����ԴĿ�˿ںŴ洢�����ײ� */
	if(ntohs(udp_hdr->srcport) == 53 || ntohs(udp_hdr->dstport) == 53)
	{
		saveDNS(pkt_data, offset + 8);
		pList1->SetItemText(list_rows, ++list_cols, "DNS"); 
	}
	else if( (ntohs(udp_hdr->srcport) == 67 && ntohs(udp_hdr->dstport) == 68) || (ntohs(udp_hdr->srcport) == 68 && ntohs(udp_hdr->dstport) == 67))
	{
		pList1->SetItemText(list_rows, ++list_cols, "DHCP"); 
	}

	else
	{
		pList1->SetItemText(list_rows, ++list_cols, "UDP"); 
	}
}

/* �洢TCP�� */
void saveTCP(const u_char *pkt_data, int offset)
{
	tcp_header *tcp_hdr = (tcp_header*)(pkt_data + offset);

	/* �洢tcp�ײ���������*/
	tcp_header *p;

	p = (tcp_header*)malloc(sizeof(tcp_header));
	p->srcport = tcp_hdr->srcport;
	p->dstport = tcp_hdr->dstport;
	p->seq = tcp_hdr->seq;
	p->ack = tcp_hdr->ack;
	p->hdrlen_rsv_flags = tcp_hdr->hdrlen_rsv_flags;
	p->win_size = tcp_hdr->win_size;
	p->chksum = tcp_hdr->chksum;
	p->urg_ptr = tcp_hdr->urg_ptr;
	p->option = tcp_hdr->option;

	linklist.GetTail().tcph = p;

	/* ����ԴĿ�˿ںŴ洢�����ײ� */
	if(ntohs(tcp_hdr->srcport) == 53 || ntohs(tcp_hdr->dstport) == 53)
	{
		saveDNS(pkt_data, offset + (ntohs(tcp_hdr->hdrlen_rsv_flags) >> 12) * 4);
		pList1->SetItemText(list_rows, ++list_cols, "DNS"); 
	}

	else if(ntohs(tcp_hdr->srcport) == 80 || ntohs(tcp_hdr->dstport) == 80)
	{	
		pList1->SetItemText(list_rows, ++list_cols, "HTTP"); 
	}

	else
	{
		pList1->SetItemText(list_rows, ++list_cols, "TCP"); 
	}
	
}

/* �洢DNS */
void saveDNS(const u_char *pkt_data, int offset)
{
	dns_header *dns_hdr = (dns_header*)(pkt_data + offset);

	/* �洢dns�ײ��������� */
	dns_header *p;
	p = (dns_header*)malloc(sizeof(dns_header));
	p->identifier = dns_hdr->identifier;
	p->flags = dns_hdr->flags;
	p->questions = dns_hdr->questions;
	p->answers = dns_hdr->answers;
	p->authority = dns_hdr->authority;
	p->additional = dns_hdr->additional;

	linklist.GetTail().dnsh = p;
}

/* ������̫��֡ */
void decodeFrame(mac_address *saddr, mac_address *daddr, u_short *eth_type, HTREEITEM *hParent)
{
	HTREEITEM hFrameItem;		//���οؼ����
	CString str1, str2;

	str1 = "��̫��֡ ��";
	str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", saddr->byte1, saddr->byte2, saddr->byte3, saddr->byte4, saddr->byte5, saddr->byte6);
	str1 += str2 + " -> ";
	str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", daddr->byte1, daddr->byte2, daddr->byte3, daddr->byte4, daddr->byte5, daddr->byte6);
	str1 += str2 + "��";
	hFrameItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);
	
	
	str1 = "Դmac��ַ��";
	str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", saddr->byte1, saddr->byte2, saddr->byte3, saddr->byte4, saddr->byte5, saddr->byte6);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hFrameItem, 0);

	str1 = "Ŀ��mac��ַ��";
	str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", daddr->byte1, daddr->byte2, daddr->byte3, daddr->byte4, daddr->byte5, daddr->byte6);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hFrameItem, 0);

	str1 = "���ͣ�";
	switch(*eth_type)
	{
	case 0x0800: str2 = "IPv4 (0x0800)"; break;
	case 0x0806: str2 = "ARP (0x0806)"; break;
	default: str2.Format("Unknown(0x%04hx)", *eth_type);	break;
	}
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hFrameItem, 0);

}

/* ����IP  */
void decodeIP(ip_header *iph,HTREEITEM *hParent)
{
		HTREEITEM hIPItem;		//���οؼ����
		CString str1, str2;

		str1 = "IP ��";
		str2.Format("%d.%d.%d.%d", iph->srcaddr.byte1, iph->srcaddr.byte2, iph->srcaddr.byte3, iph->srcaddr.byte4);
		str1 += str2 + " -> ";
		str2.Format("%d.%d.%d.%d", iph->dstaddr.byte1, iph->dstaddr.byte2, iph->dstaddr.byte3, iph->dstaddr.byte4);
		str1 += str2 + "��";
		hIPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

		str1 = "�汾�ţ�";
		str2.Format("%d", iph->ver_hrdlen >> 4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "�ײ����ȣ�";
		str2.Format("%d (bytes)", (iph->ver_hrdlen & 0x0f) * 4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "����������";
		str2.Format("0x%02x", iph->tos);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0 ,hIPItem, 0);

		str1 = "�ܳ��ȣ� ";
		str2.Format("%hu", ntohs(iph->totallen));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0 ,hIPItem, 0);

		str1 = "��ʶ��";
		str2.Format("0x%04hx(%hu)", ntohs(iph->identifier), ntohs(iph->identifier));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0 ,hIPItem, 0);
			

		str1 = "��־��";
		str2.Format("0x%02x", ntohs(iph->flags_offset) >> 13);
		str1 += str2;
		HTREEITEM hIPFlag = pTree->InsertItem(str1, 0, 0 ,hIPItem, 0);

		str1 = "RSV������λ����0";
		pTree->InsertItem(str1, 0, 0, hIPFlag, 0);

		str1 = "MF��";
		str2.Format("%d", (ntohs(iph->flags_offset) >> 14) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPFlag, 0);

		str1 = "DF��";
		str2.Format("%d", (ntohs(iph->flags_offset) >> 13) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPFlag, 0);


		
		
		str1 = "Ƭƫ�ƣ�";
		str2.Format("%d", ntohs(iph->flags_offset) & 0x1fff);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0 ,hIPItem, 0);

		str1 = "TTL��";
		str2.Format("%u", iph->ttl);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "Э�飺";
		switch(iph->proto)
		{
		case 1:	str2 = "ICMP (1)"; break;
		case 6:	str2 = "TCP (6)"; break;
		case 17: str2 = "UDP (17)"; break;
		default: str2.Format("UNKNOWN(%d)", iph->proto);	break;
		}

		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "У��ͣ�";
		str2.Format("0x%02hx", ntohs(iph->checksum));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "Դip��ַ��";
		str2.Format("%d.%d.%d.%d", iph->srcaddr.byte1, iph->srcaddr.byte2, iph->srcaddr.byte3, iph->srcaddr.byte4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

		str1 = "Ŀ��ip��ַ��";
		str2.Format("%d.%d.%d.%d", iph->dstaddr.byte1, iph->dstaddr.byte2, iph->dstaddr.byte3, iph->dstaddr.byte4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hIPItem, 0);

}

/* ����ARP */
void decodeARP(arp_header *arph, HTREEITEM *hParent)
{
	HTREEITEM hARPItem;			//���οؼ����
	CString str1, str2;

	str1 = "ARP ��";
		switch(ntohs(arph->op))
		{
		case 1:	str2.Format("Request"); break;
		case 2:	str2.Format("Reply");	break;
		}
		str1 += str2 + "��";		
		hARPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

		str1 = "Ӳ�����ͣ�";
		str2.Format("%hu", ntohs(arph->hardtype), ntohs(arph->hardtype));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Э�����ͣ�";
		str2.Format("0x%04hx (%hu)", ntohs(arph->prototype), ntohs(arph->prototype));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Ӳ����ַ���ȣ�";
		str2.Format("%u", arph->hardlen);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Э���ַ���ȣ�";
		str2.Format("%u", arph->protolen);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "OP��";
		switch(ntohs(arph->op))
		{
		case 1:	str2.Format("0x%04hx (Request)", ntohs(arph->op)); break;
		case 2:	str2.Format("0x%04hx (Reply)", ntohs(arph->op));	break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Դmac��ַ��";
		str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", arph->srcmac.byte1, arph->srcmac.byte2, arph->srcmac.byte3, arph->srcmac.byte4, arph->srcmac.byte5, arph->srcmac.byte1);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Դip��ַ��";
		str2.Format("%d.%d.%d.%d", arph->srcip.byte1, arph->srcip.byte2, arph->srcip.byte3, arph->srcip.byte4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);
		
		str1 = "Ŀ��mac��ַ��";
		str2.Format("%02X-%02X-%02X-%02X-%02X-%02X", arph->dstmac.byte1, arph->dstmac.byte2, arph->dstmac.byte3, arph->dstmac.byte4, arph->dstmac.byte5, arph->dstmac.byte1);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

		str1 = "Ŀ��ip��ַ��";
		str2.Format("%d.%d.%d.%d", arph->dstip.byte1, arph->dstip.byte2, arph->dstip.byte3, arph->dstip.byte4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hARPItem, 0);

}

/* ����ICMP */
void decodeICMP(icmp_header *icmph, HTREEITEM *hParent)
{
	HTREEITEM hICMPItem;
	CString str1, str2;

	str1 = "ICMP ��";

	switch(icmph->type)
	{
		case 0: str2 = "��ӦӦ�𱨸�"; break;
		case 3: str2 = "���޲��ɴﱨ��"; break;
		case 4: str2 = "Դ�����Ʊ���"; break;
		case 5: str2 = "�ض��򱨸�"; break;
		case 8: str2 = "��Ӧ���󱨸�"; break;
		case 9: str2 = "·����ͨ�汨��"; break;
		case 10: str2 = "·����ѯ�ʱ���"; break;
		case 11: str2 = "��ʱ����"; break;
		case 12: str2 = "���ݱ��������󱨸�"; break;
		case 13: str2 = "ʱ������󱨸�"; break;
		case 14: str2 = "ʱ���Ӧ�𱨸�"; break;
		case 17: str2 = "��ַ�������󱨸�"; break;
		case 18: str2 = "��ַ����Ӧ�𱨸�"; break;
		default: str2.Format("UNKNOWN��%d��", icmph->type); break;
	}

	str1 += str2 + "��";

	hICMPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

	ip_address *addr = (ip_address*)(&(icmph->others));

	u_short id = (u_short)(ntohl(icmph->others) >> 16);
	u_short seq = (u_short)(ntohl(icmph->others) & 0x0000ffff);

	str1 = "���ͣ�";
	switch(icmph->type)
	{
	case 3: str2 = "3"; 
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "���룺";
			switch(icmph->code)
			{
			case 0: str2 = "0 �����粻�ɴ"; break;
			case 1: str2 = "1 ���������ɴ"; break;
			case 2: str2 = "2 ��Э�鲻�ɴ"; break;
			case 3: str2 = "3 ���˿ڲ��ɴ"; break;
			case 6: str2 = "6 ����������δ֪��"; break;
			case 7: str2 = "7 ����������δ֪��"; break;
			default: str2.Format("%d ��UNKNOWN��", icmph->code); break;
			}
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			break;

	case 4: str2 = "4";
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "���룺0 ";
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);
			
			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);
			break;

	case 5: str2 = "5";
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "���룺";
			switch(icmph->code)
			{
			case 0:	str2 = "0 �����ض������ض���"; break;
			case 1: str2 = "1 �����ض������ض���"; break;
			case 2: str2 = "2 ������ָ���ķ������Ͷ��ض������ض���";break;
			case 3: str2 = "3 ������ָ���ķ������Ͷ��ض������ض���"; break;
			}
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);


			str1 = "Ŀ��·������IP��ַ��";
			str2.Format("%d.%d.%d.%d", addr->byte1, addr->byte2, addr->byte3, addr->byte4);
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);
			break;

	case 11: str2 = "11"; 
			 str1 += str2;
			 pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			 str1 = "���룺";
			 switch(icmph->code)
			 {
			 case 0: str2 = "0 ��TTL��ʱ��";	break;
			 case 1: str2 = "1 ����Ƭ���鳬ʱ��"; break;
			 }
			 str1 += str2;
			 pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			 break;

	case 8: str2 = "8";
			 str1 += str2;
			 pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "���룺0";
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "��ʶ��";
			str2.Format("%hu", id);
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "��ţ�";
			str2.Format("%hu", seq);
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			break;

	case 0:	str2 = "0";
			str1 += str2;
		    pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "���룺0";
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "У��ͣ�";
			str2.Format("0x%04hx", ntohs(icmph->chksum));
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "��ʶ��";
			str2.Format("%hu", id);
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			str1 = "��ţ�";
			str2.Format("%hu", seq);
			str1 += str2;
			pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			break;

	default: str2.Format("%d", icmph->type);
			 str1 += str2;
			 pTree->InsertItem(str1, 0, 0, hICMPItem, 0);

			 str1 = "���룺";
			 str2.Format("%d", icmph->code);

			 str1 = "У��ͣ�";
			 str2.Format("0x%04hx", icmph->chksum);
			 str1 += str2;
			 pTree->InsertItem(str1, 0, 0, hICMPItem, 0);
			 break;
	}
}


/* ����UDP */
void decodeUDP(udp_header *udph, HTREEITEM *hParent)
{
		HTREEITEM hUDPItem;		//���οؼ����
		CString str1, str2;

		str1 = "UDP ��";
		str2.Format("%hu", ntohs(udph->srcport));
		str1 += str2 + " -> ";
		str2.Format("%hu", ntohs(udph->dstport));
		str1 += str2 + "��";
		hUDPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

		str1 = "Դ�˿ڣ�";
		str2.Format("%hu", ntohs(udph->srcport));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hUDPItem, 0);

		str1 = "Ŀ�Ķ˿ڣ�";
		str2.Format("%hu", ntohs(udph->dstport));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hUDPItem, 0);

		str1 = "���ȣ�";
		str2.Format("%hu", ntohs(udph->len));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hUDPItem, 0);

		str1 = "У��ͣ�";
		str2.Format("0x%04hx", ntohs(udph->checksum));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hUDPItem, 0);

}

/* ����TCP */
void decodeTCP(tcp_header *tcph, HTREEITEM *hParent)
{
		HTREEITEM hTCPItem;		//���οؼ����
		CString str1, str2;

		str1 = "TCP ��";
		str2.Format("%d", ntohs(tcph->srcport));
		str1 += str2 + " -> ";
		str2.Format("%d", ntohs(tcph->dstport));
		str1 += str2 + "��";
		hTCPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

		str1 = "Դ�˿ڣ�";
		str2.Format("%hu", ntohs(tcph->srcport));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hTCPItem, 0);

		str1 = "Ŀ�Ķ˿ڣ�";
		str2.Format("%hu", ntohs(tcph->dstport));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hTCPItem, 0);

		str1 = "���кţ�";
		str2.Format("%lu", ntohl(tcph->seq));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hTCPItem, 0);

		str1 = "ȷ�Ϻţ�";
		str2.Format("%lu", ntohl(tcph->ack));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);

		str1 = "�ײ����ȣ�";
		str2.Format("%d (bytes)", (ntohs(tcph->hdrlen_rsv_flags) >> 12 ) * 4);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);

		str1 = "��־��";
		str2.Format("0x%03x", ntohs(tcph->hdrlen_rsv_flags) & 0x0fff);
		str1 += str2;
		HTREEITEM hTCPFlag = pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);

		str1 = "URG��";
		str2.Format("%d", (ntohs(tcph->hdrlen_rsv_flags) >> 5) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "ACK��";
		str2.Format("%d", (ntohs(tcph->hdrlen_rsv_flags) >> 4) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "PSH��";
		str2.Format("%d", (ntohs(tcph->hdrlen_rsv_flags) >> 3) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "RST��";
		str2.Format("%d", (ntohs(tcph->hdrlen_rsv_flags) >> 2) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "SYN��";
		str2.Format("%d", (ntohs(tcph->hdrlen_rsv_flags) >> 1) & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "FIN��";
		str2.Format("%d", ntohs(tcph->hdrlen_rsv_flags)  & 0x0001);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,hTCPFlag, 0);

		str1 = "���ڴ�С��";
		str2.Format("%hu", ntohs(tcph->win_size));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);

		str1 = "У��ͣ�";
		str2.Format("0x%04hx", ntohs(tcph->chksum));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);

		str1 = "����ָ�룺";
		str2.Format("%hu", ntohs(tcph->urg_ptr));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0,  hTCPItem, 0);


}


/* ����DNS  offsetΪ��dns�ײ���ƫ���� */
void decodeDNS(u_char *pkt_data, int offset, dns_header *dnsh, HTREEITEM *hParent)
{
		HTREEITEM hDNSItem;			//���οؼ����
		CString str1, str2;

		str1 = "DNS ��";
		switch(dnsh->flags >> 15)
		{
		case 0:	str2 = "Query��";		break;
		case 1:	str2 = "Response��";	break;
		}
		str1 += str2;
		hDNSItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

		str1 = "��ʶ��";
		str2.Format("0x%04hx (%hu)", ntohs(dnsh->identifier), ntohs(dnsh->identifier)); 
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		str1 = "��־��";
		str2.Format("0x%04hx", ntohs(dnsh->flags));
		str1 += str2;

		HTREEITEM hDNSFlag = pTree->InsertItem(str1, 0, 0, hDNSItem, 0);
		/* ��־���ֶ� */
		str1 = "QR��";
		switch(ntohs(dnsh->flags) >> 15)
		{
		case 0: str2 = "0 ����ѯ���ģ�"	;	break;
		case 1: str2 = "1 ����Ӧ���ģ�";	break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "OpCode��";
		switch((ntohs(dnsh->flags) >> 11) & 0x000f)
		{
		case 0: str2 = "0 ����׼��ѯ��";	break;
		case 1:	str2 = "1 �������ѯ��";	break;
		case 2: str2 = "2 ��������״̬����"; break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "AA��";
		switch((ntohs(dnsh->flags) >> 10) & 0x0001)
		{
		case 0:	str2 = "0 ������Ȩ�ش�"; break;
		case 1: str1 = "1 ����Ȩ�ش�"; break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "TC��";
		switch((ntohs(dnsh->flags) >> 9) & 0x0001)
		{
		case 0: str2 = "0 ������δ�ضϣ�"; break;
		case 1: str2 = "1 �����Ľضϣ�";	break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "RD��";
		switch((ntohs(dnsh->flags) >> 8) & 0x0001)
		{
		case 0: str2 = "0"; break;
		case 1: str2 = "1 ��ϣ�����еݹ��ѯ��";	break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "RA��";
		switch((ntohs(dnsh->flags) >> 7) & 0x0001)
		{
		case 0: str2 = "0 ����������֧�ֵݹ��ѯ��"; break;
		case 1: str2 = "1 ��������֧�ֵݹ��ѯ��";	break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "Reserved��";
		str2.Format("%d", (ntohs(dnsh->flags) >> 4) & 0x0007);
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "rCode��";
		switch(ntohs(dnsh->flags)  & 0x000f)
		{
		case 0: str2 = "0 ���޲��";		break;
		case 1: str2 = "1 ����ʽ���";	break;	
		case 2: str2 = "2 ��DNS���������⣩";	break;
		case 3: str2 = "3 �����������ڻ����";	break;
		case 4: str2 = "4 ����ѯ���Ͳ�֧�֣�";	break;
		case 5: str2 = "5 ���ڹ����Ͻ�ֹ��";	break;
		default: str2.Format("%d��������", ntohs(dnsh->flags) & 0x000f);				break;
		}
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSFlag, 0);

		str1 = "��ѯ��¼����";
		str2.Format("%hu", ntohs(dnsh->questions));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		str1 = "�ش��¼����";
		str2.Format("%hu", ntohs(dnsh->answers));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		str1 = "��Ȩ�ش��¼����";
		str2.Format("%hu", ntohs(dnsh->authority));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		str1 = "������Ϣ��¼����";
		str2.Format("%hu", ntohs(dnsh->additional));
		str1 += str2;
		pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		str1 = "��ѯ���֣�";
		HTREEITEM hDNSQuery = pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		/* ��ѯ���� */
		char *p = (char*)(pkt_data + offset + 12);	

		int query_num = 0, answer_num = 0, authority_num = 0, additional_num = 0;

		if(ntohs(dnsh->questions) < 10)
		{
			while(query_num < ntohs(dnsh->questions))
			{
				char *name1 = (char*)malloc(strlen(p)+1);

				translateName(name1, p);
			
				/* ���������ֶ� */
				while(*p)
				{
					++p;
				}
				++p;

				str1.Format("%s", name1);
				str1 += "��";

				dns_query *dnsq = (dns_query*)p;
				u_short	type, classes;

				type = ntohs(dnsq->type);
				classes = ntohs(dnsq->classes);
				
				switch(type)
				{
				case 1:	str2 = "type A"; break;
				case 2:	str2 = "type NS"; break;
				case 5: str2 = "type CNAME"; break;
				case 6: str2 = "type SOA"; break;
				case 12: str2 = "type PTR"; break;
				case 15: str2 = "type MX"; break;
				case 28: str2 = "type AAAA"; break;
				case 255: str2 = "type ANY"; break;
				default: str2.Format("type UNKNOWN(%hu)", type); break;
				}
				str1 += str2 + ", ";

				switch(classes)
				{
				case 1: str2 = "class INTERNET"; break;
				case 2: str2 = "class CSNET";	break;
				case 3: str2 = "class COAS";	break;
				default: str2.Format("class UNKNOWN(%hu)", classes); break;
				}
				str1 += str2;

				pTree->InsertItem(str1, 0, 0, hDNSQuery, 0);

				/* ������ѯ���ͺͲ�ѯ���ֶ� */
				p += sizeof(dns_query);

				query_num++;
				free(name1);
			}
		}

		str1 = "�ش𲿷֣�";
		HTREEITEM hDNSAnswer = pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		/* �ش𲿷� */
		while(answer_num < ntohs(dnsh->answers))
		{

			/* ָ��ָ�� */
			if(*(u_char*)p == 0xc0)
			{
				
				/* ָ��ƫ���� 		
				++p;	
				
				char *name = (char*)(pkt_data + offset + *(u_char*)p);			// ����
				char *name1 = (char*)malloc(strlen(name)+1);

			
				translateName(name1, name);
				
				str1.Format("%s", name1);
				str1 += "��";

  				free(name1);
				*/
					
				char name[70];
				char name1[70];

				translateData(pkt_data, offset, name, p, 2);
				translateName(name1, name);

				str1.Format("%s", name1);
				str1 += "��";

				/* ָ��ƫ���� */
				++p;


				/* ָ������*/
				++p;
				dns_answer *dnsa = (dns_answer*)p;

				u_short type =  ntohs(dnsa->type);
				u_short classes = ntohs(dnsa->classes);
				u_long  ttl  = ntohl(dnsa->ttl);

				switch(type)
				{
				case 1:	str2 = "type A"; break;
				case 2:	str2 = "type NS"; break;
				case 5: str2 = "type CNAME"; break;
				case 6: str2 = "type SOA"; break;
				case 12: str2 = "type PTR"; break;
				case 15: str2 = "type MX"; break;
				case 28: str2 = "type AAAA"; break;
				case 255: str2 = "type ANY"; break;
				default: str2.Format("type UNKNOWN(%hu)", type); break;
				}
				str1 += str2 + ", ";

				switch(classes)
				{
				case 1: str2 = "class INTERNET"; break;
				case 2: str2 = "class CSNET";	break;
				case 3: str2 = "class COAS";	break;
				default: str2.Format("class UNKNOWN(%hu)", classes); break;
				}
				str1 += str2 + ", ";

				str2.Format("ttl %lu", ttl);
				str1 += str2 + ", ";

				/* ָ����Դ���ݳ��� */
				p += sizeof(dns_answer);
				
				u_short data_len = ntohs(*(u_short*)p);

				str2.Format("len %hu", data_len);
				str1 += str2 + ", ";

				/* ָ����Դ���� */
				p += sizeof(u_short);

				/* ��ѯ����ΪNS��CNAME��PTR����Դ���� */
				if(type == 2 || type == 5 || type == 12)
				{
	
					/* ��Դ����Ϊָ��0xc0 + ƫ����*/
					if(*(u_char*)p == 0xc0)
					{				
						/* ����ƫ������ȡ���� 											
						char *data = (char*)(pkt_data + offset + *(u_char*)(p+1));			// ����
						char *data1 = (char*)malloc(strlen(data)+1);

						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;

						free(data1);
						*/
						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, 2);
						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;

					}
					/* ��Դ���ݴ���ָ��0xc0 + ƫ���� */
					else if(isNamePtr(p))
					{
						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, data_len);		// ȥ��ָ��0xc0+ƫ����
						translateName(data1, data);								// ȥ��'.'

						str2.Format("%s", data1);
						str1 += str2;
					}
					/* ��Դ�����в�����ָ��0xc0 + ƫ���� */
					else
					{
						char *data = (char*)malloc(data_len);

						translateName(data, p);

						str2.Format("%s", data);
						str1 += str2;
						free(data);
						
					}
				}
				/* ��ѯ����ΪA����Դ���� */
				else if(type == 1)
				{
					ip_address data = *(ip_address*)p;

					str2.Format("%d.%d.%d.%d", data.byte1, data.byte2, data.byte3, data.byte4);
					str1 += str2;
				}

				pTree->InsertItem(str1, 0, 0, hDNSAnswer, 0);
			
				/* �������ݲ��� */
				p += data_len;

		

			}//if
			answer_num++;
		}

		str1 = "��Ȩ�ش𲿷֣�";
		HTREEITEM hDNSAuthority = pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		/* ��Ȩ�ش𲿷� */
		while(authority_num < ntohs(dnsh->authority))
		{

			/* ָ��ָ�� */
			if(*(u_char*)p == 0xc0)
			{
				
				/* ָ��ƫ���� 		
				++p;	
				
				char *name = (char*)(pkt_data + offset + *(u_char*)p);			// ����
				char *name1 = (char*)malloc(strlen(name)+1);
				translateName(name1, name);
				
				str1.Format("%s", name1);
				str1 += "��";

				free(name1);
				*/
				char name[70];
				char name1[70];

				translateData(pkt_data, offset, name, p, 2);
				translateName(name1, name);

				str1.Format("%s", name1);
				str1 += "��";

				/* ָ��ƫ���� */
				++p;

				/* ָ������*/
				++p;
				dns_answer *dnsa = (dns_answer*)p;

				u_short type =  ntohs(dnsa->type);
				u_short classes = ntohs(dnsa->classes);
				u_long  ttl  = ntohl(dnsa->ttl);

				switch(type)
				{
				case 1:	str2 = "type A"; break;
				case 2:	str2 = "type NS"; break;
				case 5: str2 = "type CNAME"; break;
				case 6: str2 = "type SOA"; break;
				case 12: str2 = "type PTR"; break;
				case 15: str2 = "type MX"; break;
				case 28: str2 = "type AAAA"; break;
				case 255: str2 = "type ANY"; break;
				default: str2.Format("type UNKNOWN(%hu)", type); break;
				}
				str1 += str2 + ", ";

				switch(classes)
				{
				case 1: str2 = "class INTERNET"; break;
				case 2: str2 = "class CSNET";	break;
				case 3: str2 = "class COAS";	break;
				default: str2.Format("class UNKNOWN(%hu)", classes); break;
				}
				str1 += str2 + ", ";

				str2.Format("ttl %lu", ttl);
				str1 += str2 + ", ";

				/* ָ����Դ���ݳ��� */
				p += sizeof(dns_answer);
				
				u_short data_len = ntohs(*(u_short*)p);

				str2.Format("len %hu", data_len);
				str1 += str2 + ", ";

				/* ָ����Դ���� */
				p += sizeof(u_short);

				/* ��ѯ����ΪNS��CNAME��PTR����Դ���� */
				if(type == 2 || type == 5 || type == 12)
				{
	
					/* ��Դ����Ϊָ��0xc0 + ƫ����*/
					if(*(u_char*)p == 0xc0)
					{				
						/* ����ƫ������ȡ���� 											
						char *data = (char*)(pkt_data + offset + *(u_char*)(p+1));			// ����
						char *data1 = (char*)malloc(strlen(data)+1);

						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;

						free(data1);
						*/

						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, 2);
						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;
					}
					/* ��Դ���ݴ���ָ��0xc0 + ƫ���� */
					else if(isNamePtr(p))
					{
						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, data_len);		// ȥ��ָ��0xc0+ƫ����
						translateName(data1, data);								// ȥ��'.'

						str2.Format("%s", data1);
						str1 += str2;
					}
					/* ��Դ�����в�����ָ��0xc0 + ƫ���� */
					else
					{
						char *data = (char*)malloc(data_len);

						translateName(data, p);

						str2.Format("%s", data);
						str1 += str2;
						free(data);
						
					}
				}
				/* ��ѯ����ΪA����Դ���� */
				else if(type == 1)
				{
					ip_address data = *(ip_address*)p;

					str2.Format("%d.%d.%d.%d", data.byte1, data.byte2, data.byte3, data.byte4);
					str1 += str2;
				}

				pTree->InsertItem(str1, 0, 0, hDNSAuthority, 0);
			
				/* �������ݲ��� */
				p += data_len;


			}//if
			authority_num++;
		}

		str1 = "������Ϣ���֣�";
		HTREEITEM hDNSAdditional = pTree->InsertItem(str1, 0, 0, hDNSItem, 0);

		/* ������Ϣ���� */
		while(additional_num < ntohs(dnsh->additional))
		{

			/* ָ��ָ�� */
			if(*(u_char*)p == 0xc0)
			{
				
				/* ָ��ƫ���� 		
				++p;	
				
				char *name = (char*)(pkt_data + offset + *(u_char*)p);			// ����
				char *name1 = (char*)malloc(strlen(name)+1);

				translateName(name1, name);
				
				str1.Format("%s", name1);
				str1 += "��";

				free(name1);
				*/
				char name[70];
				char name1[70];

				translateData(pkt_data, offset, name, p, 2);
				translateName(name1, name);

				str1.Format("%s", name1);
				str1 += "��";

				/* ָ��ƫ���� */
				++p;

				/* ָ������*/
				++p;
				dns_answer *dnsa = (dns_answer*)p;

				u_short type =  ntohs(dnsa->type);
				u_short classes = ntohs(dnsa->classes);
				u_long  ttl  = ntohl(dnsa->ttl);

				switch(type)
				{
				case 1:	str2 = "type A"; break;
				case 2:	str2 = "type NS"; break;
				case 5: str2 = "type CNAME"; break;
				case 6: str2 = "type SOA"; break;
				case 12: str2 = "type PTR"; break;
				case 15: str2 = "type MX"; break;
				case 28: str2 = "type AAAA"; break;
				case 255: str2 = "type ANY"; break;
				default: str2.Format("type UNKNOWN(%hu)", type); break;
				}
				str1 += str2 + ", ";

				switch(classes)
				{
				case 1: str2 = "class INTERNET"; break;
				case 2: str2 = "class CSNET";	break;
				case 3: str2 = "class COAS";	break;
				default: str2.Format("class UNKNOWN(%hu)", classes); break;
				}
				str1 += str2 + ", ";

				str2.Format("ttl %lu", ttl);
				str1 += str2 + ", ";

				/* ָ����Դ���ݳ��� */
				p += sizeof(dns_answer);
				
				u_short data_len = ntohs(*(u_short*)p);

				str2.Format("len %hu", data_len);
				str1 += str2 + ", ";

				/* ָ����Դ���� */
				p += sizeof(u_short);

				/* ��ѯ����ΪNS��CNAME��PTR����Դ���� */
				if(type == 2 || type == 5 || type == 12)
				{
	
					/* ��Դ����Ϊָ��0xc0 + ƫ����*/
					if(*(u_char*)p == 0xc0)
					{				
						/* ����ƫ������ȡ���� 											
						char *data = (char*)(pkt_data + offset + *(u_char*)(p+1));			// ����
						char *data1 = (char*)malloc(strlen(data)+1);

						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;

						free(data1);
						*/

						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, 2);
						translateName(data1, data);

						str2.Format("%s", data1);
						str1 += str2;
					}
					/* ��Դ���ݴ���ָ��0xc0 + ƫ���� */
					else if(isNamePtr(p))
					{
						char data[70];
						char data1[70];

						translateData(pkt_data, offset, data, p, data_len);		// ȥ��ָ��0xc0+ƫ����
						translateName(data1, data);								// ȥ��'.'

						str2.Format("%s", data1);
						str1 += str2;
					}
					/* ��Դ�����в�����ָ��0xc0 + ƫ���� */
					else
					{
						char *data = (char*)malloc(data_len);

						translateName(data, p);

						str2.Format("%s", data);
						str1 += str2;
						free(data);
						
					}
				}
				/* ��ѯ����ΪA����Դ���� */
				else if(type == 1)
				{
					ip_address data = *(ip_address*)p;

					str2.Format("%d.%d.%d.%d", data.byte1, data.byte2, data.byte3, data.byte4);
					str1 += str2;
				}

				pTree->InsertItem(str1, 0, 0, hDNSAdditional, 0);
			
				/* �������ݲ��� */
				p += data_len;

			}//if
			additional_num++;
		}		
}

/* ����HTTP offsetΪ��HTTP���ĵ�ƫ����*/
void decodeHTTP(u_char *pkt_data, int offset, HTREEITEM *hParent)
{
	u_char *p = (pkt_data + offset);
	ip_header *iph = (ip_header*)(pkt_data + 14);
	tcp_header *tcph = (tcp_header*)(pkt_data + 14 + (iph->ver_hrdlen & 0x0f) *4);

	int http_len = ntohs(iph->totallen) - (iph->ver_hrdlen & 0x0f) * 4 - (ntohs(tcph->hdrlen_rsv_flags) >> 12)*4;
	int count = 0;

	CString str1;
	
	str1 = "HTTP";
	HTREEITEM hHTTPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

	
	while( count < http_len)
	{
		str1 = "";
		while(*p != '\r')
		{
			str1 += *p;
			++p;
			++count;
		}
		str1 += "\\r\\n";
		pTree->InsertItem(str1, 0, 0, hHTTPItem, 0);

		p += 2;
		count += 2;
	}	
}


/* ����DHCP offsetΪ��DHCP���ĵ�ƫ����*/
void decodeDHCP(u_char *pkt_data, int offset, HTREEITEM *hParent)
{
	dhcp_header *dhcph = (dhcp_header*)(pkt_data + offset);
	u_char *p = (u_char*)(pkt_data + offset + sizeof(dhcp_header));	//pָ��ͻ���Ӳ����ַ

	CString str1, str2;

	str1 = "DHCP";
	HTREEITEM hDHCPItem = pTree->InsertItem(str1, 0, 0, *hParent, 0);

	/* ����dhcp�ײ� */
	str1 = "�������ͣ�";
	str2.Format("%d", dhcph->op);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

	str1 = "Ӳ�����ͣ�";
	str2.Format("%d", dhcph->htype);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

	str1 = "Ӳ����ַ���ȣ�";
	str2.Format("%d", dhcph->hlen);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

	str1 = "������";
	str2.Format("%d", dhcph->hops);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

	str1 = "����ID��";
	str2.Format("0x%08lx", ntohl(dhcph->xid));
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "�ͻ�����ʱ�䣺";
	str2.Format("%hu", ntohs(dhcph->secs));
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "��־��";
	str2.Format("0x%04hx", ntohs(dhcph->flags));
	str1 += str2;
	switch(ntohs(dhcph->flags) >> 15)
	{
	case 0: str1 += "���㲥��"; break;
	case 1: str1 += "��������"; break;
	}
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "�ͻ���IP��ַ��";
	str2.Format("%d.%d.%d.%d", dhcph->ciaddr.byte1, dhcph->ciaddr.byte2, dhcph->ciaddr.byte3, dhcph->ciaddr.byte4);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "��ģ��ͻ���IP��ַ��";
	str2.Format("%d.%d.%d.%d", dhcph->yiaddr.byte1, dhcph->yiaddr.byte2, dhcph->yiaddr.byte3, dhcph->yiaddr.byte4);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "������IP��ַ��";
	str2.Format("%d.%d.%d.%d", dhcph->siaddr.byte1, dhcph->siaddr.byte2, dhcph->siaddr.byte3, dhcph->siaddr.byte4);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	str1 = "����IP��ַ��";
	str2.Format("%d.%d.%d.%d", dhcph->giaddr.byte1, dhcph->giaddr.byte2, dhcph->giaddr.byte3, dhcph->giaddr.byte4);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	/*  ����dhcp�ײ�ʣ�ಿ�� */
	mac_address *chaddr = (mac_address*)p; 
						
	str1 = "�ͻ���mac��ַ��";
	str2.Format("%02x-%02x-%02x-%02x-%02x-%02x", chaddr->byte1, chaddr->byte2, chaddr->byte3, chaddr->byte4, chaddr->byte5, chaddr->byte6);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	// �����ͻ���Ӳ����ַ
	p += 16;		

	str1 = "��������������";
	str2.Format("%s", p);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0,hDHCPItem, 0);

	// ����������������
	p += 64;		

	str1 = "�����ļ�����";
	str2.Format("%s", p);
	str1 += str2;
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

	// ���������ļ���
	p += 128;

	if(ntohl(*(u_long*)p) == 0x63825363)
	{
		str1 = "Magic cookie: DHCP";
		pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);
	}

	// ����magic cookie
	p += 4;

	while(*p != 0xff)
	{
		switch(*p)
		{
		case 53: 
			{	str1 = "ѡ���53��DHCP��������";
				 switch(*(p+2))
				 {
					case 1: str1 += "��Discover��"; break;
					case 2: str1 += "��Offer��"; break;
					case 3: str1 += "��Request��"; break;
					case 4: str1 += "��Decline��"; break;
					case 5: str1 += "��ACK��"; break;
					case 6: str1 += "��NAK��"; break;
					case 7: str1 += "��Release��"; break;
					case 8: str1 += "��Inform��"; break;
				 }
				 HTREEITEM hDHCPOption;
				 hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 str1 = "���ȣ�";
				 str2.Format("%d", *(++p));
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 str1 = "DHCP��";
				 str2.Format("%d", *(++p));
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);
				 
				 // ָ����һ��ѡ��
				 ++p;
			}
			break;

		case 50: 
			{	
				str1 = "ѡ���50������IP��ַ";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 str1 = "���ȣ�";
				 str2.Format("%d", *(++p));
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 ip_address *addr = (ip_address*)(++p);
				 str1 = "��ַ��";
				 str2.Format("%d.%d.%d.%d", addr->byte1, addr->byte2, addr->byte3, addr->byte4);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	
				 
				 // ָ����һ��ѡ��
				 p += 4;
			}
				 break;

		case 51:
			{
				str1 = "ѡ���51��IP��ַ��Լʱ��";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);	

				 str1 = "���ȣ�";
				 str2.Format("%d", *(++p));
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 u_int time = *(++p);
				 str1 = "��Լʱ�䣺";
				 str2.Format("%u", time);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 // ָ����һ��ѡ��
				 p += 4;
			}
				 break;

		case 61: 
			{
				 str1 = "ѡ���61���ͻ�����ʶ";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 str1 = "Ӳ�����ͣ�";
				 if(*(++p) == 0x01)
				 {
					str2 = "��̫����0x01��";		
					str1 += str2;
					pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

					mac_address *addr = (mac_address*)(++p);
					str1 = "�ͻ�����ʶ��";
					str2.Format("%02x-%02x-%02x-%02x-%02x-%02x", addr->byte1, addr->byte2, addr->byte3, addr->byte4, addr->byte5, addr->byte6);
					str1 += str2;
					pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

					p += 6;
				 }
				 else
				 {
					str2.Format("%d", *p);
					str1 += str2;
					pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

					p += len;

				 }	
			}
				 break;

		case 60: 
			{
				 str1 = "ѡ���60����Ӧ�����ʶ";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 int count = 0;
				 str1 = "��Ӧ�����ʶ��";

				 for(;count < len; count++)
				 {
					 str2.Format("%c", *(++p));
					 str1 += str2;
				 }
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	 

				 ++p;
			}
				 break;

		case 54: 
			{	
				 str1 = "ѡ���54����������ʶ";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 ip_address *addr = (ip_address*)(++p);
				 str1 = "��������ʶ��";
				 str2.Format("%d.%d.%d.%d", addr->byte1, addr->byte2, addr->byte3, addr->byte4);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 p += 4;
			}
				 break;

		case 1:	 
			{
				 str1 = "ѡ���1����������";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 ip_address *submask = (ip_address*)(++p);
				 str1 = "�������룺";
				 str2.Format("%d.%d.%d.%d", submask->byte1, submask->byte2, submask->byte3, submask->byte4);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 p += 4;
			}
				 break;

		case 3:  
			{
				 str1 = "ѡ���3��·����";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

				 int count = 0;
				 while( count < len)
				 {
					 ip_address *addr = (ip_address*)(++p);
					 str1 = "·������";
					 str2.Format("%d.%d.%d.%d", addr->byte1, addr->byte2, addr->byte3, addr->byte4);
					 str1 += str2;
					 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

					 count += 4;
					 p += 4;
				 }
			}
				 break;

		case 6:  
			{
				 str1 = "ѡ���6��DNS������";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	 

				 int count = 0;
				 ++p;
				 while( count < len)
				 {
					 ip_address *addr = (ip_address*)p;
					 str1 = "DNS��������";
					 str2.Format("%d.%d.%d.%d", addr->byte1, addr->byte2, addr->byte3, addr->byte4);
					 str1 += str2;
					 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	

					 count += 4;
					 p += 4;
				 }
			}
				 break;


		case 12: 
			{	
				 str1 = "ѡ���12��������";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	 

				 int count = 0;
				 str1 = "��������";

				 for(;count < len; count++)
				 {
					 str2.Format("%c", *(++p));
					 str1 += str2;
				 }
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	 

				 ++p;
			}
				 break;

		case 0: ++p;
				break;

		default: str1 = "ѡ���";
				 str2.Format("%d", *p);
				 str1 += str2 + "��";
				 HTREEITEM hDHCPOption = pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);

				 int len = *(++p);
				 str1 = "���ȣ�";
				 str2.Format("%d", len);
				 str1 += str2;
				 pTree->InsertItem(str1, 0, 0, hDHCPOption, 0);	 

				 // ָ��ѡ������
				 ++p;

				 // ����ѡ������
				 p += len;
				 break;
		}

	}
	str1 = "ѡ���255������";
	pTree->InsertItem(str1, 0, 0, hDHCPItem, 0);	 
	


}

/* �ж�data������ָ��0xc0,������ָ����data�е�λ��*/
int isNamePtr(char *data)
{
	char *p = data;
	int pos = 0;

	while(*p)
	{
		if(*(u_char*)p == 0xc0)
		{
			return pos;
		}
		++p;
		++pos;
	}

	return 0;
}
void translateName(char *name1, const char *name2)
{
	strcpy(name1, name2);

	char *p = name1;
	bool canMove = false;

	if( !isalnum(*(u_char*)p) && *(u_char*)p !=  '-')
	{
		canMove = true;
	}

	/* ������ת��Ϊ'.' */
	while(*p)
	{
		if(!isalnum(*(u_char*)p) && *(u_char*)p != '-')
			*p = '.';

		++p;
	}


	/* ������������ǰ��1λ */
	if(canMove)
	{
		p = name1;
		while(*p)
		{
			*p = *(p+1);
			++p;
		}
	}

	
}

/* DNS��Դ��¼���ݲ���ת�� ������ָ��0xc0��data2ת��Ϊ����ָ���data1 offsetΪ��dns�ײ���ƫ����*/
void translateData(u_char *pkt_data, int offset, char *data1, char *data2, int data2_len)
{
	char *p = data2;
	int count = 0, i = 0;

	/* ����data2 */
	while(count < data2_len )
	{			
		/* ָ�� */
		if(*(u_char*)p == 0xc0)
		{
			++p;

			/* ��ȡָ����ָ������� */
			char *data_ptr = (char*)(pkt_data + offset + *(u_char*)p);

			int pos;
			pos = isNamePtr(data_ptr);
			if(pos)
			{
				translateData(pkt_data, offset, data1+i, data_ptr, pos+2);
			}
			else
			{
				strcpy(data1+i, data_ptr);
				i += strlen(data_ptr)+1;
			}
			count += 2;
		}
		else 
		{
			data1[i++] = *p;
			++p;
			++count;
		}

	}
}

/* ����б��¼� */
void CSnifferUIDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	HTREEITEM hIDItem;		//���οؼ����

	int sel_row;
	u_short *eth_type;
	mac_address *saddr, *daddr;
	arp_header	*arph;
	ip_header	*iph;
	icmp_header *icmph;
	udp_header	*udph;
	tcp_header	*tcph;
	dns_header	*dnsh;

	CString str1,str2;
	

	/* ɾ�����н�� */
	m_tree.DeleteAllItems();

	/* ��ȡѡ���е��к� */
	sel_row = pList1->GetSelectionMark();

	/* ��ȡѡ���еı�����Ϣ */
	POSITION pos = linklist.FindIndex(sel_row);
	packet_header *ppkth = &linklist.GetAt(pos);
	if(ppkth == NULL)
	{
		AfxMessageBox("ppkthΪ��ָ��", MB_OK);
		return;
	}

	saddr = &ppkth->saddr;
	daddr = &ppkth->daddr;
	eth_type = &ppkth->eth_type;
	arph = ppkth->arph;
	iph = ppkth->iph;
	icmph = ppkth->icmph;
	udph = ppkth->udph;
	tcph = ppkth->tcph;
	dnsh = ppkth->dnsh;

	/* ��ӡ���ݰ����༭�� */
	int count = 0;
	u_char *p = ppkth->pkt_data;
	while(count < ppkth->caplen)
	{
		str2.Format("%02hx ", *p);
		str1 += str2;

		++p;
		++count;
	}	
	GetDlgItem(IDC_EDIT1)->SetWindowText(str1);


	/* ������Ž�� */
	str1.Format("��%d�����ݰ�", sel_row + 1);
	hIDItem = m_tree.InsertItem(str1);

	/* ������̫��֡��� */
	decodeFrame(saddr, daddr, eth_type, &hIDItem);
	m_tree.Expand(hIDItem, TVE_EXPAND);
	/* ����ip��� */
	if(iph != NULL)
	{
		decodeIP(iph, &hIDItem);		
	}
	
	/* ����arp��� */
	if(arph != NULL)
	{
		decodeARP(arph, &hIDItem);
	}

	/* ����icmp��� */
	if(icmph != NULL)
	{
		decodeICMP(icmph, &hIDItem);
	}
															
	/* ����udp��� */
	if(udph != NULL)
	{
		decodeUDP(udph, &hIDItem);	
	}

	/* ����tcp��� */
	if(tcph != NULL)
	{
		decodeTCP(tcph, &hIDItem);
	}

	/* ����dns��� */
	if(dnsh != NULL)
	{
		int offset;

		switch(iph->proto)
		{
		case 6:	 offset = 14 + (iph->ver_hrdlen & 0x0f)*4 + (ntohs(tcph->hdrlen_rsv_flags) >> 12)*4;	break;	//tcph
		case 17: offset = 14 + (iph->ver_hrdlen & 0x0f)*4 + 8 ;	break;											//udph 
			
		}
		 
															
		decodeDNS(ppkth->pkt_data, offset, dnsh, &hIDItem);	
	}

	/* ����http��� */
	if(tcph != NULL && (ntohs(tcph->srcport) == 80 || ntohs(tcph->dstport) == 80))
	{
		int offset = 14 + (iph->ver_hrdlen & 0x0f)*4 + (ntohs(tcph->hdrlen_rsv_flags) >> 12)*4;

		decodeHTTP(ppkth->pkt_data, offset,&hIDItem);
	}

	/* ����dhcp��� */
	if(udph != NULL && ( (ntohs(udph->srcport) == 67 && ntohs(udph->dstport) == 68) || (ntohs(udph->srcport) == 68 && ntohs(udph->dstport) == 67) ))
	{
		int offset = 14 + (iph->ver_hrdlen & 0x0f)*4 + 8;

		decodeDHCP(ppkth->pkt_data, offset, &hIDItem);
	}

	*pResult = 0;
}

