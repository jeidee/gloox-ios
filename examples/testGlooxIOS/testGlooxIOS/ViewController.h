//
//  ViewController.h
//  testGlooxIOS
//
//  Created by 신창헌 on 2016. 8. 28..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "GlooxInterfaceIOS.h"

@interface ViewController : UIViewController <GlooxDelegate>

@property (weak, nonatomic) IBOutlet UITextField *txtId;
@property (weak, nonatomic) IBOutlet UITextField *txtPwd;
@property (weak, nonatomic) IBOutlet UITextField *txtHost;
@property (weak, nonatomic) IBOutlet UITextField *txtPort;
@property (weak, nonatomic) IBOutlet UIButton *btnConnect;
@property (weak, nonatomic) IBOutlet UIButton *btnDisconnect;
@property (weak, nonatomic) IBOutlet UIButton *btnSend;
@property (weak, nonatomic) IBOutlet UITextField *txtToJid;
@property (weak, nonatomic) IBOutlet UITextField *txtMessage;
@property (weak, nonatomic) IBOutlet UITextView *txtLog;
@property (weak, nonatomic) IBOutlet UISwitch *swLog;

- (IBAction)doConnect:(id)sender;
- (IBAction)doRemoveLog:(id)sender;
- (IBAction)doDisconnect:(id)sender;
- (IBAction)doSend:(id)sender;

- (void)addLog:(NSString*)log;

@end

