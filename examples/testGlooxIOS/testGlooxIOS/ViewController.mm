//
//  ViewController.m
//  testGlooxIOS
//
//  Created by 신창헌 on 2016. 8. 28..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import "ViewController.h"
#include "gloox_interface.h"

#include <thread>

@interface ViewController ()

@end

@implementation ViewController

void* g_gloox = NULL;
std::thread g_recvThread;

void onLog(const char* tag, const char* log) {
    printf("%s :: %s\n", tag, log);
}

void recvLoop() {
    while(true) {
        gwRecv(g_gloox);
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    g_gloox = gwCreateInstance();
    
    gwRegisterOnLog(g_gloox, &onLog);
    
    gwConnect(g_gloox, "test1@bypass", "1234", "192.168.0.202", 5223);
    
    g_recvThread = std::thread(recvLoop);
}

- (void)viewDidDisappear:(BOOL)animated {
    //gwDeleteInstance(client);
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
