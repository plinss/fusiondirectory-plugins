<h2><img class="center" alt="" align="middle" src="images/rightarrow.png" /> {t}VoIP database information{/t}</h2>
  <table summary="">
    <tr>
     <td>{t}Asterisk DB user{/t}{$must}</td>
     <td>
{render acl=$goFonAdminACL}
      <input type='text' name="goFonAdmin" size=30 maxlength=60 id="goFonAdmin" value="{$goFonAdmin}">
{/render}
     </td>
    </tr>
    <tr>
     <td>{t}Password{/t}{$must}</td>
     <td>
{render acl=$goFonPasswordACL}
      <input type=password name="goFonPassword" id="goFonPassword" size=30 maxlength=60 value="{$goFonPassword}">
{/render}
     </td>
    </tr>
    <tr>
     <td>{t}Country dial prefix{/t}{$must}</td>
     <td>
{render acl=$goFonCountryCodeACL}
      <input type='text' name="goFonCountryCode" size=10 maxlength=30 id="goFonCountryCode" value="{$goFonCountryCode}">
{/render}
     </td>
    </tr>
    <tr>
     <td>{t}Local dial prefix{/t}{$must}</td>
     <td>
{render acl=$goFonAreaCodeACL}
      <input type='text' name="goFonAreaCode" size=10 maxlength=30 id="goFonAreaCode" value="{$goFonAreaCode}">
{/render}
     </td>
    </tr>
   </table>

<p class='seperator'>&nbsp;</p>
<div style="width:100%; text-align:right;padding-top:10px;padding-bottom:3px;">
    <input type='submit' name='SaveService' value='{msgPool type=saveButton}'>
    &nbsp;
    <input type='submit' name='CancelService' value='{msgPool type=cancelButton}'>
</div>
<input type="hidden" name="serviceAsteriskPosted" value="1">