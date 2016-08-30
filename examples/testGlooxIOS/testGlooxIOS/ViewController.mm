//
//  ViewController.m
//  testGlooxIOS
//
//  Created by 신창헌 on 2016. 8. 28..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import "ViewController.h"
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)viewWillAppear:(BOOL)animated {
    [[GlooxInterfaceIOS getInstance] addEventHandler:self];
}

- (void)viewDidDisappear:(BOOL)animated {
    [[GlooxInterfaceIOS getInstance] removeEventHandler:self];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)addLog:(NSString*)log {
    self.txtLog.text = [self.txtLog.text stringByAppendingFormat:@"%@\n", log];
    
    if(self.txtLog.text.length > 0 ) {
        NSRange bottom = NSMakeRange(self.txtLog.text.length -1, 1);
        [self.txtLog scrollRangeToVisible:bottom];
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}

- (IBAction)doConnect:(id)sender {
    
    
    NSString *jid = self.txtId.text;
    NSString *pwd = self.txtPwd.text;
    NSString *host = self.txtHost.text;
    NSString *port = self.txtPort.text;
    
    if ([[GlooxInterfaceIOS getInstance] connect:jid withPwd:pwd withHost:host withPort:[port intValue]]) {
        [self addLog:@"Connecting..."];
    } else {
        [self addLog:@"Connection failed."];
    }
}

- (IBAction)doRemoveLog:(id)sender {
    self.txtLog.text = @"";
}

- (IBAction)doDisconnect:(id)sender {
    [[GlooxInterfaceIOS getInstance] disConnect];
}

- (IBAction)doSend:(id)sender {
    [[GlooxInterfaceIOS getInstance] sendMessage:self.txtToJid.text withMsg:self.txtMessage.text];
}

#pragma mark GlooxDelegate

- (void) onConnect {
    [self addLog:@"OnConnect"];
    
    [self.btnConnect setHidden:YES];
    [self.btnDisconnect setHidden:NO];
    [self.btnSend setHidden:NO];
}

- (void) onDisconnect:(int)e {
    [self addLog:[NSString stringWithFormat:@"OnDisconnect] e: %d", e]];
    
    [self.btnConnect setHidden:NO];
    [self.btnDisconnect setHidden:YES];
    [self.btnSend setHidden:YES];
}

- (void) onLog:(NSString *)tag withLog:(NSString *)log {
    if ([self.swLog isOn]) {
        [self addLog:[NSString stringWithFormat:@"OnLog]%@ :: %@\n", tag, log]];        
    }
}

@end
