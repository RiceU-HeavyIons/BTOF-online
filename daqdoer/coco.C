int doEPS = 0;

void coco() {

TH1::AddDirectory(kFALSE);
char buf[50];
TFile *f=new TFile("daqdoer.root","READ");
f->cd();

TCanvas *coco_1 = new TCanvas("coco_1","coco_1",20,20,1000,800);
		 coco_1->Clear(); coco_1->Divide(4,4,0.001,0.001);
TH1D *hcoco_p2p[16];
TH1D *hcoco_p2p_p2p[16];
for (int i=0;i<16;i++){
	coco_1->cd(1+i);
	sprintf(buf,"hcoco_p2p%d",i);
	hcoco_p2p[i] = (TH1D*)gDirectory->Get(buf);
	hcoco_p2p[i]->GetXaxis()->SetRangeUser(80.5,130.5);
	hcoco_p2p[i]->Draw();
	//
	sprintf(buf,"hcoco_p2p_p2p%d",i);
	hcoco_p2p_p2p[i] = (TH1D*)gDirectory->Get(buf);
	hcoco_p2p_p2p[i]->Draw("same");
	//
}
coco_1->cd(); coco_1->Update(); 
if (doEPS) {
	coco_1->Print("coco_1.eps");
}



}