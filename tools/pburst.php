<?
/* Coded by Alex "Rubin" Schumann with help from Reed and Sirvulcan
*
* Released under the terms of the GPL license http://www.gpl.org
*
* No guarantees for any purpose
*/
//ob_start("ob_gzhandler");

$NUMNICKMASK = 63;
$convert2y = array(
   'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
   'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
   'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
   'w','x','y','z','0','1','2','3','4','5','6','7','8','9','[',']');
function int_to_base64($v)
{
  global $convert2y, $NUMNICKMASK;
  $count = 3;
  $buf = "xxx";
  while ($count > 0) {
    $buf[--$count] = $convert2y[($v & $NUMNICKMASK)];
    //echo "<BR>DEBUG: buf[$count] from ".($v & $NUMNICKMASK)." = $buf<BR>";
    $v >>= 6;
  }
     return $buf;

}

function int_to_dict($v, $dict)
{
    $c = count($dict);
    $left = floor($v / $c);
    $right = $v - ($left*$c);
    return($dict[$left].$dict[$right]);
       
   }

   function gen_chan_mode($i)
   {
       $i++;
       $m = "";
       if($i % 2 == 0) $m .= "n";
       if($i % 3 == 0) $m .= "t";
       if($i % 5 == 0) $m .= "r";
       if($i % 13 == 0) $m .= "s"; elseif($i % 17 == 0) $m .= "p";
       if($i % 19 == 0) $m .= "i";
       if($i % 29 == 0) $m .= "m";
       return($m);
   }

   function gen_user_mode($i)
   {
       $i++;
       $m = "";
       if($i % 2 == 0) $m .= "i";
       if($i % 13 == 0) $m .= "o";
       return($m);
   }

   function get_friend_ids($j, $start, $num)
   {
       $friends = array();
       $i = $j-$start;
       $num_in_chan = $i;
       if($num_in_chan < 5)
           $num_in_chan = 5;
       if($num_in_chan > 100)
           $num_in_chan = 100;
       $num_in_chan = $num_in_chan * (1/(($j % 201)+1));
       for($k=0;$k<$num_in_chan;$k++)
       {
           if ($k != $i)
             if ($j-$k+1 != $j)
               $friends[] = $j-$k+1;
       }
       return $friends;
   }

   function get_all_friend_ids($numofusers)
   {
       $friends = array();
       for($k=0;$k<$numofusers;$k++)
       {
          $friends[] = $k;
       }
       return $friends;
   }

   function pbsend_init()
   {
       global $output;
       $output = "";
       if($_REQUEST["asfile"])
       {
          header("Content-type: application/txt");
          header('Content-Disposition: attachment; filename="pburst.txt');
          // always modified
          header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
           
          // HTTP/1.1
          header("Cache-Control: no-cache, must-revalidate");
       }
       else
          echo "<PRE>";
   }

   function pbsend($str)
   {
       echo $str;
   }

   function pbsend_done()
   {
       flush();
   }

   if($_REQUEST["go"])
   {
       if($startnum + $numofusers > 262144)
         die("hard upper limit of 262144 would be breached by those counts<br>");

       if($_REQUEST["start"])
           $startnum = $_REQUEST["start"];
       else
           $startnum = 0;

       if($_REQUEST["num"])
           $numofusers = $_REQUEST["num"];
       else
           $numofusers = 50;

       $dict = file("/usr/share/dict/words");

       $count = 0; $j=0;
       while(1)
       {
          if((++$j) > count($dict))
               break;
          if(preg_match('/^[a-zA-Z]{3,6}[\n\m]*$/', $dict[$j]))
          {
             if(!$dupfind[ucfirst(trim($dict[$j]))])
             {
                 $mydict[] = ucfirst(trim($dict[$j]));
                 $dupfind[ucfirst(trim($dict[$j]))] = 1;
                 $count++;
             }
          }
       }
       $dict = NULL;
       $dupfind = NULL;

      $end = "\n";
      pbsend_init();
      for($i=$startnum;$i<=$startnum+$numofusers;$i++)
       {
          $nick = int_to_dict($i, $mydict);
          $host = "$nick.pburst.org";
          $num = int_to_base64($i);
          $name = $nick;
          $timestamp = time()-($i*2);
          $mode = gen_user_mode($i);
	  $mode2 = "r";
          pbsend("%N N $nick 2 $timestamp $nick $host +$mode$mode2 $name AKA$num %N$num :Number $i - $name$end");
          set_time_limit(30);
       }
       if($_REQUEST["burst"])
       {
           // BURST INTO CHANNELS?
           //servernumeric B #channel timestamp [channel modes] client[:modes],client,client,client
           for($i=$startnum;$i<=$startnum+$numofusers;$i++)
           {
              $channame = int_to_dict($i, $mydict);
              $timestamp = $timestamp = time()-($i*2);
              $modes = gen_chan_mode($i);
              $num = int_to_base64($i);
              $friends_array = get_friend_ids($i, $startnum, $numofusers);
              $friends = "";
              foreach($friends_array as $friend_int)
                $friends .= ",%N".int_to_base64($friend_int);
              pbsend("%N B #$channame $timestamp +$modes %N$num:o$friends$end");
//            set_time_limit(30);
           }

           $timestamp = $timestamp = time()-($i*2);
           $modes = gen_chan_mode($numofusers);
           $num = int_to_base64($numofusers);
           $friends_array = get_all_friend_ids($numofusers);
           $friends = "";
           $f=1;
           foreach($friends_array as $friend_int) {
             $friends .= ",%N".int_to_base64($friend_int);
             if ($f > 50) {
               pbsend("%N B #bigchannel $timestamp +nt %N$num:o$friends$end");
               $friends = "";
               $f = 1;
             } else {
               $f++;
             }
          }
       }
       //set_time_limit(3600);
       pbsend_done();
   }
   else
   {
       ?>
       <html>
       <head>
         <title>pBurst file generator for P10 networks</title>
       </head>
       <body>
       <h1>pBurst source file generator for P10 networks</h1>
       <p>Can generate up to 262,144 P10 users with word nicknames and no conflicts.
       <form>
          Start:<input type=text name="start" value="1"><br>
          Number of users: <input type=text name="num" value="100"><br>
          Burst Into Channels: <input type=checkbox name="burst" checked><br>
          Output as a file: <input type=checkbox name="asfile" checked><br>
          <input type=submit value="Go">
          <input type=hidden name=go value=1>
       </form>
       <a href="pburst.phps">Source</a>
       </body>
       </html>
       <?  
   }
   ?>
