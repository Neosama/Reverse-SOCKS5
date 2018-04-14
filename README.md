# Reverse-SOCKS5 V2

  -ymNMMMMdy/     'mm/       /hmmo'  .=Neosama=:                         :ydmmmmy+'   
 yMNs:-.-:oNMd.   .MM+    '/mMNs.    yMMsoooooo.                       'hMNy/:/omMN:  
.MMs       .mms   .MM+  '+mMNs.     'NMd                 .::.      -::':mm/     'NMh  
'hMNs/-.''        .MM+'+mMm+.       +MMhshhhy+.          .NMd'    .NMd          /MMs  
 '/ymNNNmdys/.    .MMdmMNMm:        dNNds++sdMNo          /MMo   'dMN.        .sMNy'  
     .-:+shmMN+   .MMMd/.yNMy.      ---'     oMM/          yMM-  oMM+      ':yNNh:    
hhs        'yMM.  .MMs'   :dMm/     ..'      -MMo          'dMd'-NMd'    '+dMms-      
yMM+'      .hMN.  .MM+     'oNMh.  .mNh'    'yMN:           -NMsdMN-    /mMd/.        
'omMmhs++shmMd:   .MM+       -hMN+' /mMdsoosdMd:             +MMMM+    /MMMhhhhhhhhh  
  ':osyyyys+-'    'ss-        '+so:  ':Server:'               +sso     /ssssssssssss  

Server/Client SOCKS5 (Secured Over Credential-based Kerberos 5) in Reverse mode on Windows.  
Client need Proxifier for to work.  
Defined in RFC 1928.  

Contains:
- TCP connection
- Multi-Threads
- Specials URL for execute actions in SK5_Client
- OldModem mode is a limited connection to ~2KB/sec for nostalgic surf

Specials URL list:
- "exitsoft.sk5", for kill SK5 Client
- "tetris.sk5", for play Tetris theme

Changelog:
- Better management of Threads
- Code Optimization, Comments in English only and Cleanup
- Add OldModem and Tetris theme for fun

Todo:
- IPv6 support
- Replace deprecated functions
- Add statistic page in Specials URL

This project is licensed under the GNU General Public License v3.0
