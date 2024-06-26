/*
 THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF OUTRAGE
 ENTERTAINMENT, INC. ("OUTRAGE").  OUTRAGE, IN DISTRIBUTING THE CODE TO
 END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
 ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
 IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
 SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
 FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
 CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
 AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
 COPYRIGHT 1996-2000 OUTRAGE ENTERTAINMENT, INC.  ALL RIGHTS RESERVED.
 */
#if !defined(AFX_DALLASUSERTYPESDLG_H__92061D21_B460_11D2_A4E0_00A0C96ED60D__INCLUDED_)
#define AFX_DALLASUSERTYPESDLG_H__92061D21_B460_11D2_A4E0_00A0C96ED60D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DallasUserTypesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDallasUserTypesDlg dialog

class CDallasUserTypesDlg : public CDialog {
  // Construction
public:
  CDallasUserTypesDlg(CWnd *pParent = NULL); // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDallasUserTypesDlg)
  enum { IDD = IDD_DALLAS_USERTYPES_DIALOG };
  CListBox m_ValuesListBox;
  CComboBox m_UserTypeCombo;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDallasUserTypesDlg)
protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
                                                   //}}AFX_VIRTUAL

  // Implementation
protected:
  // Generated message map functions
  //{{AFX_MSG(CDallasUserTypesDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnAddButton();
  afx_msg void OnChangeButton();
  afx_msg void OnDeleteButton();
  virtual void OnOK();
  afx_msg void OnSelchangeUtypeCombo();
  afx_msg void OnDblclkValuesList();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DALLASUSERTYPESDLG_H__92061D21_B460_11D2_A4E0_00A0C96ED60D__INCLUDED_)
