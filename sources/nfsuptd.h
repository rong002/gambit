//
// FILE: efsuptd.h -- Declarations of dialogs for dealing with NF
// supports.
//
// $Id$
//

#ifndef NFSUPTD_H
#define NFSUPTD_H
class NfgShow;
class NFSupportInspectDialog:public wxDialogBox
{
private:
	NfgShow *bns;
	gList<NFSupport *> &sups;
	int init_disp,init_cur;
	wxText *cur_dim,*disp_dim;
	wxChoice *disp_item,*cur_item;
	char *cur_str,*disp_str;
	wxStringList *support_list;
// Low level event handlers
	static void cur_func(wxChoice &ob,wxEvent &)
	{((NFSupportInspectDialog *)ob.GetClientData())->OnCur(ob.GetSelection()+1);}
	static void disp_func(wxChoice &ob,wxEvent &)
	{((NFSupportInspectDialog *)ob.GetClientData())->OnDisp(ob.GetSelection()+1);}
	static void new_sup_func(wxButton &ob,wxEvent &)
	{((NFSupportInspectDialog *)ob.GetClientData())->OnNewSupport();}
	static void change_sup_func(wxButton &ob,wxEvent &)
	{((NFSupportInspectDialog *)ob.GetClientData())->OnChangeSupport();}
	static void help_func(wxButton &,wxEvent &)
	{wxHelpContents(NFG_SUPPORTS_HELP);}
	static void close_func(wxButton &ob,wxEvent &)
	{((NfgShow *)ob.GetClientData())->SupportInspect(SUPPORT_CLOSE);}
	static void remove_sup_func(wxButton &ob,wxEvent &)
	{((NFSupportInspectDialog *)ob.GetClientData())->OnRemoveSupport();}
// High level event handlers
	void OnNewSupport(void)
	{
	if (bns->MakeSupport())
	{
		disp_item->Append(ToString(sups.Length()));
		disp_item->SetSize(-1,-1,-1,-1);
		cur_item->Append(ToString(sups.Length()));
		cur_item->SetSize(-1,-1,-1,-1);
	}
	}
	void OnRemoveSupport(void);
	void OnChangeSupport(void)
	{
	bns->SupportInspect(SUPPORT_CHANGE);init_disp=DispSup();init_cur=CurSup();
	}
	void OnCur(int cur_sup)
	{
	cur_dim->SetValue(array_to_string(sups[cur_sup]->NumStrats()));
	disp_dim->SetValue(array_to_string(sups[cur_sup]->NumStrats()));
	disp_item->SetSelection(cur_sup-1);
	}

	void OnDisp(int disp_sup)
	{disp_dim->SetValue(array_to_string(sups[disp_sup]->NumStrats()));}

// Utility funcs
	static gString array_to_string(const gArray<int> &a)
	{
	gString tmp='(';
	for (int i=1;i<=a.Length();i++) tmp+=ToString(a[i])+((i==a.Length()) ? ")" : ",");
	return tmp;
	}
public:
	NFSupportInspectDialog(gList<NFSupport *> &sups_,int cur_sup,int disp_sup,NfgShow *bns_,wxWindow *parent=0)
		: wxDialogBox(parent,"Supports"),bns(bns_),sups(sups_)
	{
  init_cur=cur_sup;init_disp=disp_sup;
	wxForm *f=new wxForm(0);
	SetLabelPosition(wxVERTICAL);
	cur_dim=new wxText(this,0,"Current",
											array_to_string(sups[cur_sup]->NumStrats()),
											-1,-1,80,-1,wxREADONLY);
	disp_dim=new wxText(this,0,"Display",
											array_to_string(sups[disp_sup]->NumStrats()),
											-1,-1,80,-1,wxREADONLY);
	support_list=wxStringListInts(sups.Length());
	cur_str=new char[10];strcpy(cur_str,ToString(cur_sup));
	disp_str=new char[10];strcpy(disp_str,ToString(disp_sup));
	wxFormItem *cur_fitem=wxMakeFormString("",&cur_str,wxFORM_CHOICE,
					new wxList(wxMakeConstraintStrings(support_list),0));
	f->Add(cur_fitem);
	f->Add(wxMakeFormMessage("      ")); // fix the spacing... not neat but..
	wxFormItem *disp_fitem=wxMakeFormString("",&disp_str,wxFORM_CHOICE,
					new wxList(wxMakeConstraintStrings(support_list),0));
	f->Add(disp_fitem);
	f->Add(wxMakeFormNewLine());
	wxFormItem *newsup_fitem=wxMakeFormButton("New",(wxFunction)new_sup_func);
	f->Add(newsup_fitem);
	wxFormItem *rmvsup_fitem=wxMakeFormButton("Remove",(wxFunction)remove_sup_func);
	f->Add(rmvsup_fitem);
	f->Add(wxMakeFormNewLine());
	wxFormItem *close_fitem=wxMakeFormButton("Close",(wxFunction)close_func);
	f->Add(close_fitem);
	wxFormItem *cngsup_fitem=wxMakeFormButton("Apply",(wxFunction)change_sup_func);
	f->Add(cngsup_fitem);
	wxFormItem *help_fitem=wxMakeFormButton("?",(wxFunction)help_func);
	f->Add(help_fitem);
	f->AssociatePanel(this);
	cur_item=(wxChoice *)cur_fitem->GetPanelItem();
	cur_item->Callback((wxFunction)cur_func);
	cur_item->SetClientData((char *)this);
	disp_item=(wxChoice *)disp_fitem->GetPanelItem();
	disp_item->SetClientData((char *)this);
	disp_item->Callback((wxFunction)disp_func);
	((wxButton *)newsup_fitem->GetPanelItem())->SetClientData((char *)this);
	((wxButton *)rmvsup_fitem->GetPanelItem())->SetClientData((char *)this);
	((wxButton *)cngsup_fitem->GetPanelItem())->SetClientData((char *)this);
	((wxButton *)help_fitem->GetPanelItem())->SetClientData((char *)this);
	((wxButton *)close_fitem->GetPanelItem())->SetClientData((char *)bns);
	Fit();
	Show(TRUE);
	}
	// Data Access members
	int 	CurSup(void) {return cur_item->GetSelection()+1;}
	int 	DispSup(void) {return disp_item->GetSelection()+1;}
};

// Note can not delete the first support
void NFSupportInspectDialog::OnRemoveSupport(void)
{
  int i;
  SupportRemoveDialog SRD(this,sups.Length());
  if (SRD.Completed()==wxOK)  {
	 gArray<bool> selected(SRD.Selected());
	 bool revert=false;
	 for (i=sups.Length();i>=2;i--)
		if (selected[i])  {
			delete sups.Remove(i);
			if (i==init_cur || i==init_disp && revert==false)  {
			  wxMessageBox("Display/Current support deleted.\nReverting to full support");
			  revert=true;
			}
		}
	 disp_item->Clear();cur_item->Clear();
	 for (i=1;i<=sups.Length();i++)
		{disp_item->Append(ToString(i));cur_item->Append(ToString(i));}
	 disp_item->SetSize(-1,-1,-1,-1);cur_item->SetSize(-1,-1,-1,-1);
	 disp_item->SetSelection(0);cur_item->SetSelection(0);
	 if (revert) bns->SupportInspect(SUPPORT_CHANGE);
  }
}

#endif
